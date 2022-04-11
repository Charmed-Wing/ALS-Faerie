#include "AlsCharacterExample.h"

#include "AlsCameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

AAlsCharacterExample::AAlsCharacterExample()
{
	AlsCamera = CreateDefaultSubobject<UAlsCameraComponent>(TEXT("AlsCamera"));
	AlsCamera->SetupAttachment(GetMesh());
	AlsCamera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});
}

void AAlsCharacterExample::NotifyControllerChanged()
{
	const APlayerController* PreviousPlayer {Cast<APlayerController>(PreviousController)};
	if (IsValid(PreviousPlayer))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem {
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer())
		};
		if (IsValid(EnhancedInputSubsystem))
		{
			EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}

	APlayerController* NewPlayer {Cast<APlayerController>(GetController())};
	if (IsValid(NewPlayer))
	{
		NewPlayer->InputYawScale_DEPRECATED = 1.0f;
		NewPlayer->InputPitchScale_DEPRECATED = 1.0f;
		NewPlayer->InputRollScale_DEPRECATED = 1.0f;

		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer());
		if (IsValid(EnhancedInputSubsystem))
		{
			EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	Super::NotifyControllerChanged();
}

void AAlsCharacterExample::CalcCamera(const float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (AlsCamera->IsActive())
	{
		AlsCamera->GetViewInfo(ViewInfo);
		return;
	}

	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AAlsCharacterExample::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	UEnhancedInputComponent* EnhancedInput {Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::InputLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::InputSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::InputWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::InputCrouch);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::InputJump);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::InputAim);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::InputRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::InputRoll);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::InputRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::InputViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this,
		                          &ThisClass::InputSwitchShoulder);
	}
}

void AAlsCharacterExample::InputLookMouse(const FInputActionValue& ActionValue)
{
	const FVector2D Value {ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}

void AAlsCharacterExample::InputLook(const FInputActionValue& ActionValue)
{
	const FVector2D Value {ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpRate * GetWorld()->GetDeltaSeconds());
	AddControllerYawInput(Value.X * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void AAlsCharacterExample::InputMove(const FInputActionValue& ActionValue)
{
	const FVector InputValue = UAlsMath::ClampMagnitude01(ActionValue.Get<FVector>());
	//const FVector2D Value = UAlsMath::ClampMagnitude012D(ActionValue.Get<FVector2D>());

	const FVector Forward = GetViewState().Rotation.Vector();
	//const FVector ForwardDirection = UAlsMath::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw));
	const FVector RightDirection = UAlsMath::PerpendicularCounterClockwiseXY(Forward);
	const FVector UpDirection = FVector::UpVector;

	AddMovementInput(Forward * InputValue.Y + RightDirection * InputValue.X + UpDirection * InputValue.Z);
}

void AAlsCharacterExample::InputSprint(const FInputActionValue& ActionValue)
{
	SetDesiredGait(ActionValue.Get<bool>() ? EAlsGait::Sprinting : EAlsGait::Running);
}

void AAlsCharacterExample::InputWalk()
{
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	// ReSharper disable once CppIncompleteSwitchStatement
	switch (GetDesiredGait())
	{
	case EAlsGait::Walking:
		SetDesiredGait(EAlsGait::Running);
		break;

	case EAlsGait::Running:
		SetDesiredGait(EAlsGait::Walking);
		break;
	}
}

void AAlsCharacterExample::InputCrouch()
{
	switch (GetDesiredStance())
	{
	case EAlsStance::Standing:
		SetDesiredStance(EAlsStance::Crouching);
		break;

	case EAlsStance::Crouching:
		SetDesiredStance(EAlsStance::Standing);
		break;
	}
}

void AAlsCharacterExample::InputJump(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (TryStopRagdolling())
		{
			return;
		}

		if (TryStartMantlingGrounded())
		{
			return;
		}

		if (GetStance() == EAlsStance::Crouching)
		{
			SetDesiredStance(EAlsStance::Standing);
			return;
		}

		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AAlsCharacterExample::InputAim(const FInputActionValue& ActionValue)
{
	SetDesiredAiming(ActionValue.Get<bool>());
}

void AAlsCharacterExample::InputRagdoll()
{
	if (!TryStopRagdolling())
	{
		StartRagdolling();
	}
}

void AAlsCharacterExample::InputRoll()
{
	// @todo-drakyn parameterize
	static constexpr float PlayRate {1.3f};

	TryStartRolling(PlayRate);
}

void AAlsCharacterExample::InputRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == EAlsRotationMode::LookingDirection
		                       ? EAlsRotationMode::VelocityDirection
		                       : EAlsRotationMode::LookingDirection);
}

void AAlsCharacterExample::InputViewMode()
{
	SetViewMode(GetViewMode() == EAlsViewMode::ThirdPerson ? EAlsViewMode::FirstPerson : EAlsViewMode::ThirdPerson);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AAlsCharacterExample::InputSwitchShoulder()
{
	AlsCamera->SetRightShoulder(!AlsCamera->IsRightShoulder());
}

void AAlsCharacterExample::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused,
                                        float& VerticalLocation)
{
	if (AlsCamera->IsActive())
	{
		AlsCamera->DisplayDebug(Canvas, DebugDisplay, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalLocation);
}