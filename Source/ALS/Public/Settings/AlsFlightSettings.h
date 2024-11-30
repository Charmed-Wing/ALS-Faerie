#pragma once

#include "AlsFlightSettings.generated.h"

USTRUCT(BlueprintType)
struct FAlsFlightSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	float FlightInterruptThreshold{600.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TEnumAsByte<ECollisionChannel> FlightTraceChannel{ECC_WorldStatic};
};