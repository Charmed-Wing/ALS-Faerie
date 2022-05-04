#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "State/Enumerations/AlsGait.h"
#include "State/Enumerations/AlsRotationMode.h"
#include "State/Enumerations/AlsStance.h"
#include "State/Enumerations/AlsViewMode.h"
#include "State/Structures/AlsFeetState.h"
#include "State/Structures/AlsGroundedState.h"
#include "State/Structures/AlsInAirState.h"
#include "State/Structures/AlsLayeringState.h"
#include "State/Structures/AlsLeanState.h"
#include "State/Structures/AlsLocomotionAnimationState.h"
#include "State/Structures/AlsPoseState.h"
#include "State/Structures/AlsRagdollingAnimationState.h"
#include "State/Structures/AlsRotateInPlaceState.h"
#include "State/Structures/AlsTransitionsState.h"
#include "State/Structures/AlsTurnInPlaceState.h"
#include "State/Structures/AlsViewAnimationState.h"
#include "AlsAnimationInstance.generated.h"

class UAlsAnimationInstanceSettings;
class AAlsCharacter;

UCLASS()
class ALS_API UAlsAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAlsAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	// Core

protected:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	UAlsAnimationInstanceSettings* GetSettingsUnsafe() const;

public:
	void SetPendingUpdate(bool bNewPendingUpdate);

	void SetAnimationCurvesRelevant(bool bNewRelevant);

private:
	void RefreshLayering();

	void RefreshPose();

	// View

private:
	void RefreshViewGameThread();

	void RefreshView(float DeltaTime);

protected:
	virtual bool IsSpineRotationAllowed();

	// Locomotion

private:
	void RefreshLocomotionGameThread();

	void RefreshLocomotion(float DeltaTime);

	// Grounded

public:
	void SetGroundedEntryMode(const FGameplayTag& NewModeTag);

protected:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected))
	void ResetGroundedEntryMode();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void SetHipsDirection(EAlsHipsDirection NewDirection);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void ActivatePivot();

private:
	void RefreshGroundedGameThread();

	void RefreshGrounded(float DeltaTime);

	void RefreshMovementDirection();

	void RefreshVelocityBlend(float DeltaTime);

	void RefreshRotationYawOffsets();

	void RefreshSprint(const FVector3f& RelativeAccelerationAmount, float DeltaTime);

	void RefreshStrideBlendAmount();

	void RefreshWalkRunBlendAmount();

	void RefreshStandingPlayRate();

	void RefreshCrouchingPlayRate();

	void RefreshGroundedLeanAmount(const FVector3f& RelativeAccelerationAmount, float DeltaTime);

	void ResetGroundedLeanAmount(float DeltaTime);

	// In Air

public:
	void Jump();

protected:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected))
	void ResetJumped();

private:
	void RefreshInAirGameThread();

	void RefreshInAir(float DeltaTime);

	void RefreshGroundPredictionAmount();

	void RefreshInAirLeanAmount(float DeltaTime);

	// Feet

private:
	void RefreshFeetGameThread();

	void RefreshFeet(float DeltaTime);

	void RefreshFoot(FAlsFootState& FootState, const FName& FootIkCurveName, const FName& FootLockCurveName,
	                 const FTransform& ComponentTransformInverse, float DeltaTime, FVector& TargetLocationOffset) const;

	void ProcessFootLockTeleport(FAlsFootState& FootState) const;

	void ProcessFootLockBaseChange(FAlsFootState& FootState, const FTransform& ComponentTransformInverse) const;

	void RefreshFootLock(FAlsFootState& FootState, const FName& FootLockCurveName, const FTransform& ComponentTransformInverse,
	                     float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation) const;

	void RefreshFootOffset(FAlsFootState& FootState, float DeltaTime, FVector& TargetLocationOffset,
	                       FVector& FinalLocation, FQuat& FinalRotation) const;

	void RefreshPelvisOffset(float TargetFootLeftLocationOffsetZ, float TargetFootRightLocationOffsetZ, float DeltaTime);

	// Transitions

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionAnimation(UAnimSequenceBase* Animation, float BlendInTime = 0.2f, float BlendOutTime = 0.2f,
	                             float PlayRate = 1.0f, float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionLeftAnimation(float BlendInTime = 0.2f, float BlendOutTime = 0.2f, float PlayRate = 1.0f,
	                                 float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionRightAnimation(float BlendInTime = 0.2f, float BlendOutTime = 0.2f, float PlayRate = 1.0f,
	                                  float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void StopTransitionAndTurnInPlaceAnimations(float BlendOutTime = 0.2f);

private:
	void RefreshTransitionsGameThread();

	void RefreshTransitions();

	void RefreshDynamicTransition();

	void PlayQueuedDynamicTransitionAnimation();

	// Rotate In Place

protected:
	virtual bool IsRotateInPlaceAllowed();

private:
	void RefreshRotateInPlace(float DeltaTime);

	// Turn In Place

protected:
	virtual bool IsTurnInPlaceAllowed();

private:
	void RefreshTurnInPlaceGameThread();

	void RefreshTurnInPlace(float DeltaTime);

	void PlayQueuedTurnInPlaceAnimation();

	// Ragdolling

private:
	void RefreshRagdollingGameThread();

public:
	void StopRagdolling();

protected:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected))
	void FinalizeRagdolling() const;

	// Utility

public:
	float GetCurveValueClamped01(const FName& CurveName) const;

	bool IsCharacterInAir() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsAnimationInstanceSettings> Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AAlsCharacter> Character;

	// Used to indicate that the animation instance has not been updated for a long time
	// and its current state may not be correct (such as foot location used in foot locking).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bPendingUpdateGameThread {true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bPendingUpdate {true};

	// The animation curves will be relevant if the character is rendered or VisibilityBasedAnimTickOption
	// is set to AlwaysTickPoseAndRefreshBones, otherwise the curves will have their old values even though
	// the animation blueprint continues to update.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bAnimationCurvesRelevantGameThread;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bAnimationCurvesRelevant;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bTeleportedGameThread;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bTeleported;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bDisplayDebugTraces {false};

	TArray<TFunction<void()>> DisplayDebugTracesQueue;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsStanceCache Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsGaitCache Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsRotationModeCache RotationMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsViewModeCache ViewMode {EAlsViewMode::ThirdPerson};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag OverlayMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag GroundedEntryMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsLayeringState LayeringState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsPoseState PoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsViewAnimationState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsLeanState LeanState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsLocomotionAnimationState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsGroundedState GroundedState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsInAirState InAirState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsFeetState FeetState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsTransitionsState TransitionsState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsRotateInPlaceState RotateInPlaceState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsTurnInPlaceState TurnInPlaceState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsRagdollingAnimationState RagdollingState;
};

inline UAlsAnimationInstanceSettings* UAlsAnimationInstance::GetSettingsUnsafe() const
{
	return Settings;
}

inline void UAlsAnimationInstance::SetPendingUpdate(const bool bNewPendingUpdate)
{
	bPendingUpdateGameThread = bNewPendingUpdate;
}

inline void UAlsAnimationInstance::SetAnimationCurvesRelevant(const bool bNewRelevant)
{
	bAnimationCurvesRelevantGameThread = bNewRelevant;
}

inline void UAlsAnimationInstance::SetGroundedEntryMode(const FGameplayTag& NewModeTag)
{
	GroundedEntryMode = NewModeTag;
}

inline void UAlsAnimationInstance::ResetGroundedEntryMode()
{
	GroundedEntryMode = FGameplayTag::EmptyTag;
}

inline void UAlsAnimationInstance::SetHipsDirection(const EAlsHipsDirection NewDirection)
{
	GroundedState.HipsDirection = NewDirection;
}

inline void UAlsAnimationInstance::ActivatePivot()
{
	GroundedState.bPivotActivationRequested = true;
}

inline void UAlsAnimationInstance::Jump()
{
	InAirState.bJumpRequested = true;
}

inline void UAlsAnimationInstance::ResetJumped()
{
	InAirState.bJumped = false;
}