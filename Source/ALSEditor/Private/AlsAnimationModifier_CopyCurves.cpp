﻿#include "AlsAnimationModifier_CopyCurves.h"

#include "Animation/AnimSequence.h"

void UAlsAnimationModifier_CopyCurves::OnApply_Implementation(UAnimSequence* Sequence)
{
	UAnimSequence* SourceSequenceObject {SourceSequence.LoadSynchronous()};
	if (!IsValid(SourceSequenceObject))
	{
		return;
	}

	if (bCopyAllCurves)
	{
		for (const FFloatCurve& Curve : SourceSequenceObject->GetCurveData().FloatCurves)
		{
			CopyCurve(SourceSequenceObject, Sequence, Curve.Name.DisplayName);
		}
	}
	else
	{
		for (const FName& CurveName : CurveNames)
		{
			if (UAnimationBlueprintLibrary::DoesCurveExist(SourceSequenceObject, CurveName,
			                                               ERawCurveTrackTypes::RCT_Float))
			{
				CopyCurve(SourceSequenceObject, Sequence, CurveName);
			}
		}
	}
}

void UAlsAnimationModifier_CopyCurves::CopyCurve(UAnimSequence* SourceSequence, UAnimSequence* TargetSequence,
                                                 const FName& CurveName)
{
	static TArray<float> CurveTimes;
	check(CurveTimes.Num() <= 0)

	static TArray<float> CurveValues;
	check(CurveValues.Num() <= 0)

	if (UAnimationBlueprintLibrary::DoesCurveExist(TargetSequence, CurveName, ERawCurveTrackTypes::RCT_Float))
	{
		UAnimationBlueprintLibrary::RemoveCurve(TargetSequence, CurveName);
	}

	UAnimationBlueprintLibrary::AddCurve(TargetSequence, CurveName);

	UAnimationBlueprintLibrary::GetFloatKeys(SourceSequence, CurveName, CurveTimes, CurveValues);
	UAnimationBlueprintLibrary::AddFloatCurveKeys(TargetSequence, CurveName, CurveTimes, CurveValues);

	CurveTimes.Reset();
	CurveValues.Reset();
}
