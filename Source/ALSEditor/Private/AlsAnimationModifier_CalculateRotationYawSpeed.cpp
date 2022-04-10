﻿#include "AlsAnimationModifier_CalculateRotationYawSpeed.h"

#include "Animation/AnimSequence.h"
#include "Utility/AlsConstants.h"

void UAlsAnimationModifier_CalculateRotationYawSpeed::OnApply_Implementation(UAnimSequence* Sequence)
{
	if (UAnimationBlueprintLibrary::DoesCurveExist(Sequence, UAlsConstants::RotationYawSpeedCurve(),
	                                               ERawCurveTrackTypes::RCT_Float))
	{
		UAnimationBlueprintLibrary::RemoveCurve(Sequence, UAlsConstants::RotationYawSpeedCurve());
	}

	UAnimationBlueprintLibrary::AddCurve(Sequence, UAlsConstants::RotationYawSpeedCurve());

	const double FrameRate {Sequence->GetSamplingFrameRate().AsDecimal()};

	FTransform CurrentPoseTransform;
	FTransform NextPoseTransform;

	for (int32 i = 0; i < Sequence->GetNumberOfSampledKeys(); i++)
	{
		UAnimationBlueprintLibrary::GetBonePoseForFrame(Sequence, UAlsConstants::RootBone(), i, false,
		                                                CurrentPoseTransform);

		UAnimationBlueprintLibrary::GetBonePoseForFrame(Sequence, UAlsConstants::RootBone(),
		                                                i + (Sequence->RateScale >= 0.0f ? 1 : -1),
		                                                false, NextPoseTransform);

		UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, UAlsConstants::RotationYawSpeedCurve(),
		                                             Sequence->GetTimeAtFrame(i),
		                                             UE_REAL_TO_FLOAT(
			                                             NextPoseTransform.Rotator().Yaw - CurrentPoseTransform.Rotator(
			                                             ).Yaw) *
		                                             FMath::Abs(Sequence->RateScale) * FrameRate);
	}
}
