#pragma once

#include "AnimGraphNode_Base.h"
#include "Node/AlsAnimNode_CurvesBlend.h"
#include "AlsAnimGraphNode_CurvesBlend.generated.h"

UCLASS()
class ALSEDITOR_API UAlsAnimGraphNode_CurvesBlend : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual FText GetTooltipText() const override;

	virtual FString GetNodeCategory() const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FAlsAnimNode_CurvesBlend Node;
};