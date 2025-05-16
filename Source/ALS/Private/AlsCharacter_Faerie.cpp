#include "AlsCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Settings/AlsCharacterSettings.h"
#include "Utility/AlsConstants.h"

void AAlsCharacter::SetFlightMode(const FGameplayTag& NewFlightMode)
{
	if (FlightMode != NewFlightMode)
	{
		// If we are trying for a mode other than turning flight off, verify the character is able to fly.
		if (NewFlightMode != FGameplayTag::EmptyTag)
		{
			if (!CanFly()) return;
		}

		const FGameplayTag Prev = FlightMode;
		FlightMode = NewFlightMode;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, FlightMode, this)

		OnFlightModeChanged(Prev);

		if (FlightMode == FGameplayTag::EmptyTag) // We want to stop flight.
		{
			// Setting the movement mode to falling is pretty safe. If the character is grounded, than the movement
			// component will know to set it to Walking instead.
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		else if (Prev == FGameplayTag::EmptyTag) // We want to start flight.
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		else // Changing from one flight mode to another logic:
		{
			// Currently blank
		}

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerSetFlightMode(NewFlightMode);
		}
	}
}

void AAlsCharacter::OnReplicate_FlightMode(const FGameplayTag& PreviousFlightMode)
{
	OnFlightModeChanged(PreviousFlightMode);
}

void AAlsCharacter::OnFlightModeChanged_Implementation(const FGameplayTag& PreviousModeTag)
{
}

void AAlsCharacter::ServerSetFlightMode_Implementation(const FGameplayTag& NewFlightMode)
{
	SetFlightMode(NewFlightMode);
}

bool AAlsCharacter::CanFly() const
{
	return GetMovementComponent()->CanEverFly() && FlightCheck();
}

bool AAlsCharacter::FlightCheck_Implementation() const
{
	return true;
}

bool AAlsCharacter::FlightInterruptCheck_Implementation(const FHitResult& Hit)
{
	// If we touched a walkable surface with our feet, then Land.
	if (Hit.ImpactPoint.Equals(GetActorLocation() - FVector(0.0, 0.0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), 100.f))
	{
		if (GetCharacterMovement()->IsWalkable(Hit))
		{
			return true;
		}
	}


	// Otherwise, if we hit the surface with a greater force than what can be sustained, cut flight.
	float VelLen;
	FVector VelDir;
	GetVelocity().GetAbs().ToDirectionAndLength(VelDir, VelLen);

	// Dot the impact with our velocity to determine how "head-on" the collision was.
	VelLen *= VelDir | Hit.ImpactNormal;

	return VelLen >= Settings->Flying.FlightInterruptThreshold;
}

// ReSharper disable once CppMemberFunctionMayBeConst
float AAlsCharacter::FlightTrace(float Distance, const FVector& Direction)
{
	UWorld* World = GetWorld();
	if (!World) return 0.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const FVector CheckStart = GetActorLocation() - FVector{0, 0,
								GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
	const FVector CheckEnd = CheckStart + (Direction * Distance);
	bool bHit = World->LineTraceSingleByChannel(HitResult, CheckStart, CheckEnd, Settings->Flying.FlightTraceChannel, Params);

	/*if ()
	{
		DrawDebugLineTraceSingle(World, CheckStart, CheckEnd, EDrawDebugTrace::ForOneFrame,
			bHit, HitResult, FLinearColor::Yellow, FLinearColor::Red, 0.f);
	}*/

	if (bHit && HitResult.bBlockingHit) { return HitResult.Distance; }
	return Distance;
}

void AAlsCharacter::RefreshFlyingRotation(float DeltaTime)
{
	if (LocomotionAction.IsValid() || LocomotionMode != AlsLocomotionModeTags::Flying)
	{
		return;
	}

	if (!LocomotionState.bMoving)
	{
		// Not moving.

		ApplyRotationYawSpeedAnimationCurve(DeltaTime);

		if (RefreshCustomFlyingNotMovingRotation(DeltaTime))
		{
			return;
		}

		if (RotationMode == AlsRotationModeTags::Aiming || ViewMode == AlsViewModeTags::FirstPerson)
		{
			RefreshFlyingNotMovingAimingRotation(DeltaTime);
			return;
		}

		if (RotationMode == AlsRotationModeTags::VelocityDirection)
		{
			// Rotate to the last target yaw angle when not moving (relative to the movement base or not).

			const auto TargetYawAngle{
				MovementBase.bHasRelativeLocation && !MovementBase.bHasRelativeRotation &&
				Settings->bInheritMovementBaseRotationInVelocityDirectionRotationMode
					? FRotator3f::NormalizeAxis(LocomotionState.TargetYawAngle + MovementBase.DeltaRotation.Yaw)
					: LocomotionState.TargetYawAngle
			};

			static constexpr auto RotationInterpolationSpeed{12.0f};
			static constexpr auto TargetYawAngleRotationSpeed{800.0f};

			SetRotationExtraSmooth(TargetYawAngle, DeltaTime, RotationInterpolationSpeed, TargetYawAngleRotationSpeed);
			return;
		}

		RefreshTargetYawAngleUsingActorRotation();
		return;
	}

	// Moving.

	if (RefreshCustomFlyingMovingRotation(DeltaTime))
	{
		return;
	}

	if (RotationMode == AlsRotationModeTags::VelocityDirection &&
	    (LocomotionState.bHasInput || !LocomotionState.bRotationTowardsLastInputDirectionBlocked))
	{
		LocomotionState.bRotationTowardsLastInputDirectionBlocked = false;

		static constexpr auto TargetYawAngleRotationSpeed{800.0f};

		SetRotationExtraSmooth(
			Settings->bRotateTowardsDesiredVelocityInVelocityDirectionRotationMode
				? DesiredVelocityYawAngle
				: LocomotionState.VelocityYawAngle,
			DeltaTime, CalculateGroundedMovingRotationInterpolationSpeed(), TargetYawAngleRotationSpeed);
		return;
	}

	if (RotationMode == AlsRotationModeTags::ViewDirection)
	{
		const auto TargetYawAngle{
			Gait == AlsGaitTags::Sprinting
				? LocomotionState.VelocityYawAngle
				: UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw +
					GetMesh()->GetAnimInstance()->GetCurveValue(UAlsConstants::RotationYawOffsetCurveName()))
		};

		static constexpr auto TargetYawAngleRotationSpeed{500.0f};

		SetRotationExtraSmooth(TargetYawAngle, DeltaTime, CalculateGroundedMovingRotationInterpolationSpeed(), TargetYawAngleRotationSpeed);
		return;
	}

	if (RotationMode == AlsRotationModeTags::Aiming)
	{
		RefreshFlyingMovingAimingRotation(DeltaTime);
		return;
	}

	RefreshTargetYawAngleUsingActorRotation();
}

void AAlsCharacter::RefreshSwimmingRotation(float DeltaTime)
{
}

void AAlsCharacter::RefreshFlyingMovingAimingRotation(const float DeltaTime)
{
	// @todo for now do same as grounded
	RefreshGroundedAimingRotation(DeltaTime);
}

void AAlsCharacter::RefreshFlyingNotMovingAimingRotation(const float DeltaTime)
{
	// @todo for now do same as grounded
	RefreshGroundedAimingRotation(DeltaTime);
}

bool AAlsCharacter::RefreshCustomFlyingMovingRotation(float DeltaTime)
{
	return false;
}

bool AAlsCharacter::RefreshCustomFlyingNotMovingRotation(float DeltaTime)
{
	return false;
}