﻿#include "AlsAnimationModifier_CreateLayeringCurves.h"

#include "Animation/AnimSequence.h"

void UAlsAnimationModifier_CreateLayeringCurves::OnApply_Implementation(UAnimSequence* Sequence)
{
	CreateCurves(Sequence, CurveNames, CurveValue);

	if (bAddSlotCurves)
	{
		CreateCurves(Sequence, SlotCurveNames, SlotCurveValue);
	}
}

void UAlsAnimationModifier_CreateLayeringCurves::CreateCurves(UAnimSequence* Sequence, const TArray<FName>& Names,
                                                              const float Value) const
{
	for (const FName& CurveName : Names)
	{
		if (UAnimationBlueprintLibrary::DoesCurveExist(Sequence, CurveName, ERawCurveTrackTypes::RCT_Float))
		{
			if (!bOverrideExistingCurves)
			{
				continue;
			}

			UAnimationBlueprintLibrary::RemoveCurve(Sequence, CurveName);
		}

		UAnimationBlueprintLibrary::AddCurve(Sequence, CurveName);

		if (bAddKeyOnEachFrame)
		{
			for (int32 i = 0; i < Sequence->GetNumberOfSampledKeys(); i++)
			{
				UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, CurveName, Sequence->GetTimeAtFrame(i), Value);
			}
		}
		else
		{
			UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, CurveName, Sequence->GetTimeAtFrame(0), Value);
		}
	}
}