// Copyright Guy (Drakynfly) Lundvall. All Rights Reserved.

#include "ALSPhysicalAnimationComponent.h"

void UALSPhysicalAnimationComponent::EnableByProfile(const FName Bone, const FName Profile, const bool IncludeSelf,
														const bool Mirrored)
{
	ensureMsgf(IsValid(GetAnimatingMesh()), TEXT("UALSPhysicalAnimationComponent was not assigned an animating mesh when requested to enable profile!"));

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

void UALSPhysicalAnimationComponent::ClearPhysicalAnimData() const
{
	if (GetAnimatingMesh())
	{
		ClearPhysicalAnimDataBelow("root", true);
	}
}

void UALSPhysicalAnimationComponent::ClearPhysicalAnimDataBelow(const FName Bone, const bool IncludeSelf) const
{
	GetSkeletalMesh()->SetAllBodiesBelowSimulatePhysics(Bone, false, IncludeSelf);
	//GetSkeletalMesh()->SetAllBodiesBelowPhysicsBlendWeight(Bone, 0.f);
}

USkeletalMeshComponent* UALSPhysicalAnimationComponent::GetAnimatingMesh() const
{
	return GetSkeletalMesh();
}