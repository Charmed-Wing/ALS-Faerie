#pragma once

#include "AlsFlightState.generated.h"

USTRUCT(BlueprintType)
struct FAlsFlightState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LocalAltitude{0.0f};
};