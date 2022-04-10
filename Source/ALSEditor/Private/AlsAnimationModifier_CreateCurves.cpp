#include "AlsAnimationModifier_CreateCurves.h"

#include "Animation/AnimSequence.h"

void UAlsAnimationModifier_CreateCurves::OnApply_Implementation(UAnimSequence* Sequence)
{
	for (const FAlsAnimationCurve& Curve : Curves)
	{
		if (UAnimationBlueprintLibrary::DoesCurveExist(Sequence, Curve.Name, ERawCurveTrackTypes::RCT_Float))
		{
			if (!bOverrideExistingCurves)
			{
				continue;
			}

			UAnimationBlueprintLibrary::RemoveCurve(Sequence, Curve.Name);
		}

		UAnimationBlueprintLibrary::AddCurve(Sequence, Curve.Name);

		if (Curve.bAddKeyOnEachFrame)
		{
			for (int32 i {0}; i < Sequence->GetNumberOfSampledKeys(); i++)
			{
				UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, Curve.Name, Sequence->GetTimeAtFrame(i), 0);
			}
		}
		else
		{
			for (const FAlsAnimationCurveKey& CurveKey : Curve.Keys)
			{
				UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, Curve.Name,
				                                             Sequence->GetTimeAtFrame(CurveKey.Frame),
				                                             CurveKey.Value);
			}
		}
	}
}
