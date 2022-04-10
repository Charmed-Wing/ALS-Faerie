#pragma once

#include "AlsFlightSettings.generated.h"

USTRUCT(BlueprintType)
struct FAlsFlightSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseFlightInterrupt = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FlightInterruptThreshold = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> FlightTraceChannel = ECC_WorldStatic;
};