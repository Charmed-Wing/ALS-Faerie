﻿#pragma once

#include "Utility/AlsMath.h"
#include "AlsFeetState.generated.h"

USTRUCT(BlueprintType)
struct ALS_API FAlsFootState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float IkAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float LockAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BoneLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat BoneRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LockLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat LockRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LockComponentRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat LockComponentRelativeRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LockMovementBaseRelativeLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat LockMovementBaseRelativeRotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsSpringVectorState OffsetSpringState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OffsetLocation {ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat OffsetRotation {ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector IkLocation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat IkRotation{ForceInit};
};

USTRUCT(BlueprintType)
struct ALS_API FAlsFeetState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReinitializationRequired {true};

	// Choose whether a foot is planted or about to plant when stopping using the foot planted animation
	// curve. A value less than 0.5 means the foot is planted and a value more than 0.5 means the
	// foot is still in the air. The foot planted curve also determines which foot is planted (or
	// about to plant). Positive values mean the right foot is planted, negative values mean the left.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -1, ClampMax = 1))
	float FootPlantedAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 1))
	float FeetCrossingAmount {0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsFootState Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsFootState Right;

	// Pelvis

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAlsSpringFloatState PelvisSpringState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Target Pelvis Offset Z", Meta = (ForceUnits = "cm"))
	float TargetPelvisOffsetZ{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Pelvis Offset Z", Meta = (ForceUnits = "cm"))
	float PelvisOffsetZ {0.0f};
};
