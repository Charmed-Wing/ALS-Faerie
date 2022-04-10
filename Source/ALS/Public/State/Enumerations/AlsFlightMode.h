#pragma once

#include "Engine/EngineTypes.h"
#include "AlsFlightMode.generated.h"

UENUM(BlueprintType)
enum class EAlsFlightMode : uint8
{
	None,
	Hovering,
	Aerial
};

USTRUCT(BlueprintType)
struct ALS_API FAlsFlightModeCache
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	EAlsFlightMode FlightMode {EAlsFlightMode::None};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bNone {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bHovering {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool bAerial {false};

public:
	FAlsFlightModeCache() = default;

	FAlsFlightModeCache(const EAlsFlightMode InitialGait)
	{
		*this = InitialGait;
	}

	bool IsNone() const
	{
		return bNone;
	}

	bool IsHovering() const
	{
		return bHovering;
	}

	bool IsAerial() const
	{
		return bAerial;
	}

	operator EAlsFlightMode() const
	{
		return FlightMode;
	}

	void operator=(const EAlsFlightMode NewFlightState)
	{
		FlightMode = NewFlightState;

		bNone = FlightMode == EAlsFlightMode::None;
		bHovering = FlightMode == EAlsFlightMode::Hovering;
		bAerial = FlightMode == EAlsFlightMode::Aerial;
	}
};