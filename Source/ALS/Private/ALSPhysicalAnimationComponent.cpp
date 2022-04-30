// Copyright Guy (Drakynfly) Lundvall. All Rights Reserved.

#include "AlsPhysicalAnimationComponent.h"

void UAlsPhysicalAnimationComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (const FActivePhysicalAnimationSettingsLayer& Layer : Layers)
	{

	}
}

void UAlsPhysicalAnimationComponent::EnableByProfile(const FName Bone, const FName Profile, const bool IncludeSelf,
                                                     const bool Mirrored)
{
	ensureMsgf(IsValid(GetAnimatingMesh()), TEXT("UAlsPhysicalAnimationComponent was not assigned an animating mesh when requested to enable profile!"));

	FName LocalBone = Bone;

	ApplyPhysicalAnimationProfileBelow(LocalBone, Profile, IncludeSelf, true);
	GetSkeletalMesh()->SetAllBodiesBelowSimulatePhysics(LocalBone, true, IncludeSelf);

	FString BoneString = Bone.ToString();
	if (BoneString.EndsWith("_l", ESearchCase::IgnoreCase))
	{
		BoneString = BoneString.LeftChop(1).Append("r");
		LocalBone = FName(*BoneString);
		ApplyPhysicalAnimationProfileBelow(LocalBone, Profile, IncludeSelf, true);
		GetSkeletalMesh()->SetAllBodiesBelowSimulatePhysics(LocalBone, true, IncludeSelf);
	}
	else if (BoneString.EndsWith("_r", ESearchCase::IgnoreCase))
	{
		BoneString = BoneString.LeftChop(1).Append("l");
		LocalBone = FName(*BoneString);
		ApplyPhysicalAnimationProfileBelow(LocalBone, Profile, IncludeSelf, true);
		GetSkeletalMesh()->SetAllBodiesBelowSimulatePhysics(LocalBone, true, IncludeSelf);
	}
}

void UAlsPhysicalAnimationComponent::ClearPhysicalAnimData() const
{
	if (GetAnimatingMesh())
	{
		ClearPhysicalAnimDataBelow("root", true);
	}
}

void UAlsPhysicalAnimationComponent::ClearPhysicalAnimDataBelow(const FName Bone, const bool IncludeSelf) const
{
	GetSkeletalMesh()->SetAllBodiesBelowSimulatePhysics(Bone, false, IncludeSelf);
	//GetSkeletalMesh()->SetAllBodiesBelowPhysicsBlendWeight(Bone, 0.f);
}

USkeletalMeshComponent* UAlsPhysicalAnimationComponent::GetAnimatingMesh() const
{
	return GetSkeletalMesh();
}