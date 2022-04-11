#pragma once

#include "AnimationModifier.h"
#include "Utility/AlsConstants.h"
#include "AlsAnimationModifier_CreateLayeringCurves.generated.h"

UCLASS(DisplayName = "Als Create Layering Curves Animation Modifier")
class ALSEDITOR_API UAlsAnimationModifier_CreateLayeringCurves : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* Sequence) override;

private:
	void CreateCurves(UAnimSequence* Sequence, const TArray<FName>& Names, float Value) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	bool bOverrideExistingCurves = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	bool bAddKeyOnEachFrame = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurveValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<FName> CurveNames
	{
		UAlsConstants::LayerHeadCurve(),
		UAlsConstants::LayerHeadAdditiveCurve(),
		UAlsConstants::LayerArmLeftCurve(),
		UAlsConstants::LayerArmLeftAdditiveCurve(),
		UAlsConstants::LayerArmLeftLocalSpaceCurve(),
		UAlsConstants::LayerArmRightCurve(),
		UAlsConstants::LayerArmRightAdditiveCurve(),
		UAlsConstants::LayerArmRightLocalSpaceCurve(),
		UAlsConstants::LayerHandLeftCurve(),
		UAlsConstants::LayerHandRightCurve(),
		UAlsConstants::LayerSpineCurve(),
		UAlsConstants::LayerSpineAdditiveCurve(),
		UAlsConstants::LayerPelvisCurve(),
		UAlsConstants::LayerLegsCurve(),

		UAlsConstants::HandLeftIkCurve(),
		UAlsConstants::HandRightIkCurve(),

		UAlsConstants::AimManualCurve(),
		UAlsConstants::AimBlockCurve(),

		UAlsConstants::HipsDirectionLockCurve(),
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	bool bAddSlotCurves = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float SlotCurveValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<FName> SlotCurveNames
	{
		UAlsConstants::LayerHeadSlotCurve(),
		UAlsConstants::LayerArmLeftSlotCurve(),
		UAlsConstants::LayerArmRightSlotCurve(),
		UAlsConstants::LayerSpineSlotCurve(),
		UAlsConstants::LayerPelvisSlotCurve(),
		UAlsConstants::LayerLegsSlotCurve(),
	};
};