#include "AlsCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/AlsCharacterSettings.h"

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
	if (Hit.ImpactPoint.Equals(GetActorLocation() - FVector(0.0, 0.0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight())), 100.f)
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