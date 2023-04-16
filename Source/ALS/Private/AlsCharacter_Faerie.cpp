#include "AlsCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
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
	float VelLen;
	FVector VelDir;
	GetVelocity().GetAbs().ToDirectionAndLength(VelDir, VelLen);
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