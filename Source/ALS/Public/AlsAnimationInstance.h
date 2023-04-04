#pragma once

#include "Animation/AnimInstance.h"
#include "State/AlsControlRigInput.h"
#include "State/AlsFeetState.h"
#include "State/AlsGroundedState.h"
#include "State/AlsInAirState.h"
#include "State/AlsLayeringState.h"
#include "State/AlsLeanState.h"
#include "State/AlsLocomotionAnimationState.h"
#include "State/AlsMovementBaseState.h"
#include "State/AlsPoseState.h"
#include "State/AlsRagdollingAnimationState.h"
#include "State/AlsRotateInPlaceState.h"
#include "State/AlsTransitionsState.h"
#include "State/AlsTurnInPlaceState.h"
#include "State/AlsViewAnimationState.h"
#include "Utility/AlsGameplayTags.h"
#include "AlsAnimationInstance.generated.h"

class UAlsLinkedAnimationInstance;
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

	virtual void NativePostEvaluateAnimation() override;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	// Core

protected:
	UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	UAlsAnimationInstanceSettings* GetSettingsUnsafe() const;

	UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	FAlsControlRigInput GetControlRigInput() const;

public:
	void MarkPendingUpdate();

	void MarkTeleported();

private:
	void RefreshMovementBaseOnGameThread();

	void RefreshLayering();

	void RefreshPose();

	// View

public:
	virtual bool IsSpineRotationAllowed();

private:
	void RefreshViewOnGameThread();

	void RefreshView(float DeltaTime);

	void RefreshSpineRotation(float DeltaTime);

protected:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ReinitializeLookTowardsInput();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void RefreshLookTowardsInput();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ReinitializeLookTowardsCamera();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void RefreshLookTowardsCamera();

	// Locomotion

private:
	void RefreshLocomotionOnGameThread();

	// Grounded

public:
	void SetGroundedEntryMode(const FGameplayTag& NewGroundedEntryMode);

protected:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetGroundedEntryMode();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void SetHipsDirection(EAlsHipsDirection NewHipsDirection);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ActivatePivot();

private:
	void RefreshGroundedOnGameThread();

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
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetJumped();

private:
	void RefreshInAirOnGameThread();

	void RefreshInAir(float DeltaTime);

	void RefreshGroundPredictionAmount();

	void RefreshInAirLeanAmount(float DeltaTime);

	// Feet

private:
	void RefreshFeetOnGameThread();

	void RefreshFeet(float DeltaTime);

	void RefreshFoot(FAlsFootState& FootState, const FName& FootIkCurveName, const FName& FootLockCurveName,
	                 const FTransform& ComponentTransformInverse, float DeltaTime) const;

	void ProcessFootLockTeleport(FAlsFootState& FootState) const;

	void ProcessFootLockBaseChange(FAlsFootState& FootState, const FTransform& ComponentTransformInverse) const;

	void RefreshFootLock(FAlsFootState& FootState, const FName& FootLockCurveName, const FTransform& ComponentTransformInverse,
	                     float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation) const;

	void RefreshFootOffset(FAlsFootState& FootState, float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation) const;

	// Transitions

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayQuickStopAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionAnimation(UAnimSequenceBase* Animation, float BlendInDuration = 0.2f, float BlendOutDuration = 0.2f,
	                             float PlayRate = 1.0f, float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionLeftAnimation(float BlendInDuration = 0.2f, float BlendOutDuration = 0.2f, float PlayRate = 1.0f,
	                                 float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionRightAnimation(float BlendInDuration = 0.2f, float BlendOutDuration = 0.2f, float PlayRate = 1.0f,
	                                  float StartTime = 0.0f, bool bFromStandingIdleOnly = false);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void StopTransitionAndTurnInPlaceAnimations(float BlendOutDuration = 0.2f);

private:
	void RefreshTransitions();

	void RefreshDynamicTransition();

	void PlayQueuedDynamicTransitionAnimation();

	// Rotate In Place

public:
	virtual bool IsRotateInPlaceAllowed();

private:
	void RefreshRotateInPlace(float DeltaTime);

	// Turn In Place

public:
	virtual bool IsTurnInPlaceAllowed();

private:
	void RefreshTurnInPlace(float DeltaTime);

	void PlayQueuedTurnInPlaceAnimation();

	// Ragdolling

private:
	void RefreshRagdollingOnGameThread();

public:
	void StopRagdolling();

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
	bool bPendingUpdate{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bTeleported;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bDisplayDebugTraces;

	TArray<TFunction<void()>> DisplayDebugTracesQueue;
#endif
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ViewMode{AlsViewModeTags::ThirdPerson};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionMode{AlsLocomotionModeTags::Grounded};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag RotationMode{AlsRotationModeTags::LookingDirection};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Stance{AlsStanceTags::Standing};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Gait{AlsGaitTags::Walking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag OverlayMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionAction;

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


private:
	// The animation curves will be relevant if the character is rendered or VisibilityBasedAnimTickOption
	// is set to AlwaysTickPoseAndRefreshBones, otherwise the curves will have their old values even though
	// the animation blueprint continues to update.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bAnimationCurvesRelevantGameThread;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bAnimationCurvesRelevant;
};

inline UAlsAnimationInstanceSettings* UAlsAnimationInstance::GetSettingsUnsafe() const
{
	return Settings;
}

inline void UAlsAnimationInstance::MarkPendingUpdate()
{
	bPendingUpdate |= true;
}

inline void UAlsAnimationInstance::MarkTeleported()
{
	TeleportedTime = GetWorld()->GetTimeSeconds();
}

inline void UAlsAnimationInstance::SetGroundedEntryMode(const FGameplayTag& NewGroundedEntryMode)
{
	GroundedEntryMode = NewGroundedEntryMode;
}

inline void UAlsAnimationInstance::ResetGroundedEntryMode()
{
	GroundedEntryMode = FGameplayTag::EmptyTag;
}

inline void UAlsAnimationInstance::SetHipsDirection(const EAlsHipsDirection NewHipsDirection)
{
	GroundedState.HipsDirection = NewHipsDirection;
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
