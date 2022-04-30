// Copyright Guy (Drakynfly) Lundvall. All Rights Reserved.

#pragma once

#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "AlsPhysicalAnimationComponent.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class ALS_API UAlsPhysicalAnimationComponent : public UPhysicalAnimationComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Physical Animation")
	void EnableByProfile(FName Bone = "root", FName Profile = "None", bool IncludeSelf = true, bool Mirrored = false);

	UFUNCTION(BlueprintCallable, Category = "Physical Animation")
	void ClearPhysicalAnimData() const;

	UFUNCTION(BlueprintCallable, Category = "Physical Animation")
	void ClearPhysicalAnimDataBelow(FName Bone = "root", bool IncludeSelf = true) const;

	UFUNCTION(BlueprintPure, Category = "Physical Animation")
	USkeletalMeshComponent* GetAnimatingMesh() const;
};