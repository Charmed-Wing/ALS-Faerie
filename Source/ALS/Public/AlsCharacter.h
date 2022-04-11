#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Settings/AlsMantlingSettings.h"
#include "State/Enumerations/AlsGait.h"
#include "State/Enumerations/AlsFlightMode.h"
#include "State/Enumerations/AlsRotationMode.h"
#include "State/Enumerations/AlsStance.h"
#include "State/Enumerations/AlsViewMode.h"
#include "State/Structures/AlsLocomotionState.h"
#include "State/Structures/AlsRagdollingState.h"
#include "State/Structures/AlsRollingState.h"
#include "State/Structures/AlsViewState.h"
#include "AlsCharacter.generated.h"

class UAlsCharacterMovementComponent;
class UAlsCharacterSettings;
class UAlsMovementSettings;
class UAlsAnimationInstance;

UCLASS(AutoExpandCategories = ("Settings|Als Character", "Settings|Als Character|Desired State", "State|Als Character"))
class ALS_API AAlsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAlsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* Property) const override;
#endif

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void PostNetReceiveLocationAndRotation() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Restart() override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime) override final;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void Jump() override;
	virtual void OnJumped_Implementation() override;

public:
	EAlsStance GetDesiredStance() const						{ return DesiredStance; }
	EAlsStance GetStance() const							{ return Stance; }
	EAlsGait GetDesiredGait() const							{ return DesiredGait; }
	EAlsGait GetGait() const								{ return Gait; }
	bool IsDesiredAiming() const							{ return bDesiredAiming; }
	EAlsRotationMode GetDesiredRotationMode() const			{ return DesiredRotationMode; }
	EAlsRotationMode GetRotationMode() const				{ return RotationMode; }
	EAlsViewMode GetViewMode() const						{ return ViewMode; }
	EAlsFlightMode GetFlightMode() const					{ return FlightMode; }
	const FGameplayTag& GetLocomotionMode() const			{ return LocomotionMode; }
	const FGameplayTag& GetLocomotionAction() const			{ return LocomotionAction; }
	const FGameplayTag& GetOverlayMode() const				{ return OverlayMode; }
	const FVector& GetInputDirection() const				{ return InputDirection; }
	const FAlsLocomotionState& GetLocomotionState() const	{ return LocomotionState; }
	const FAlsViewState& GetViewState() const				{ return ViewState; }
	virtual FRotator GetViewRotation() const override		{ return ViewRotation; }

	bool IsInAir() const;

private:
	void PhysicsRotation(float DeltaTime);
	void ApplyDesiredStance();
	void SetStance(EAlsStance NewStance);
	void SetGait(EAlsGait NewGait);
	void RefreshGait();
	EAlsGait CalculateMaxAllowedGait() const;
	EAlsGait CalculateActualGait(EAlsGait MaxAllowedGait) const;
	bool CanSprint() const;
	void SetRotationMode(EAlsRotationMode NewMode);
	void RefreshRotationMode();
	void SetLocomotionMode(const FGameplayTag& NewModeTag);
	void NotifyLocomotionModeChanged(const FGameplayTag& PreviousModeTag);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetDesiredStance(EAlsStance NewStance);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetDesiredGait(EAlsGait NewGait);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetDesiredRotationMode(EAlsRotationMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetViewMode(EAlsViewMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetFlightMode(EAlsFlightMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewModeTag"))
	void SetOverlayMode(const FGameplayTag& NewModeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnStanceChanged(EAlsStance PreviousStance);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnGaitChanged(EAlsGait PreviousGait);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnDesiredAimingChanged(bool bPreviousDesiredAiming);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnRotationModeChanged(EAlsRotationMode PreviousMode);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFlightModeChanged(const EAlsFlightMode& PreviousMode);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredStance(EAlsStance NewStance);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGait(EAlsGait NewGait);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION()
	void OnReplicate_DesiredAiming(bool bPreviousDesiredAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredRotationMode(EAlsRotationMode NewMode);

	UFUNCTION(Server, Reliable)
	void ServerSetViewMode(EAlsViewMode NewMode);

	UFUNCTION(Server, Reliable)
	void ServerSetFlightMode(EAlsFlightMode NewMode);

	UFUNCTION()
	void OnReplicate_FlightMode(const EAlsFlightMode& PreviousMode);



protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnLocomotionModeChanged(const FGameplayTag& PreviousModeTag);

	// Locomotion Action

public:

	void SetLocomotionAction(const FGameplayTag& NewActionTag);

	void NotifyLocomotionActionChanged(const FGameplayTag& PreviousActionTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnLocomotionActionChanged(const FGameplayTag& PreviousActionTag);

	// Overlay Mode

private:
	UFUNCTION(Server, Reliable)
	void ServerSetOverlayMode(const FGameplayTag& NewModeTag);

	UFUNCTION()
	void OnReplicate_OverlayMode(const FGameplayTag& PreviousModeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnOverlayModeChanged(const FGameplayTag& PreviousModeTag);

	// View

private:
	void SetViewRotation(const FRotator& NewViewRotation);

	UFUNCTION(Server, Unreliable)
	void ServerSetViewRotation(const FRotator& NewViewRotation);

	UFUNCTION()
	void OnReplicate_ViewRotation(const FRotator& PreviousViewRotation);

	void CorrectViewInterpolation(const FRotator& PreviousViewRotation);

private:
	void RefreshView(float DeltaTime);

	void RefreshViewInterpolation(float DeltaTime);

	// Locomotion

private:
	void SetInputDirection(FVector NewInputDirection);

	FTransform CalculateNetworkSmoothedTransform() const;

	void RefreshLocomotionLocationAndRotation();

	void RefreshLocomotion(float DeltaTime);

	float FlightTrace(float Distance, const FVector& Direction);

	// Rotation

private:
	void RefreshGroundedActorRotation(float DeltaTime);

protected:
	virtual bool TryRefreshCustomGroundedMovingActorRotation(float DeltaTime);

	virtual bool TryRefreshCustomGroundedNotMovingActorRotation(float DeltaTime);

	void RefreshGroundedMovingAimingActorRotation(float DeltaTime);

	void RefreshGroundedNotMovingAimingActorRotation(float DeltaTime);

	float CalculateActorRotationInterpolationSpeed() const;

public:
	void ApplyRotationYawSpeedFromAnimationInstance(float DeltaTime);

private:
	void ApplyRotationYawSpeedFromCharacter(float DeltaTime);

	void ApplyRotationYawSpeed(float DeltaTime);

	void RefreshFallingActorRotation(float DeltaTime);

	void RefreshFlyingActorRotation(float DeltaTime);

	void RefreshSwimmingActorRotation(float DeltaTime);

protected:
	virtual bool TryRefreshCustomInAirActorRotation(float DeltaTime);

	void RefreshInAirAimingActorRotation(float DeltaTime);

protected:
	void RefreshTargetYawAngle(float TargetYawAngle);

	void RefreshViewRelativeTargetYawAngle();

	void RefreshTargetYawAngleUsingLocomotionRotation();

	void RefreshActorRotationInstant(float TargetYawAngle, ETeleportType Teleport = ETeleportType::None);

	void RefreshActorRotation(float TargetYawAngle, float DeltaTime, float ActorRotationInterpolationSpeed);

	void RefreshActorRotationExtraSmooth(float TargetYawAngle, float DeltaTime,
	                                     float ActorRotationInterpolationSpeed,
	                                     float TargetYawAngleRotationSpeed);


	// Flying

	UFUNCTION(BlueprintNativeEvent)
	bool FlightCheck();

	bool CanFly();

	UFUNCTION(BlueprintNativeEvent)
	bool FlightInterruptCheck(AActor* Actor, const FVector& Vector, const FHitResult& Hit);



	// Rotation Lock

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void LockRotation(float TargetYawAngle);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void UnLockRotation();

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLockRotation(float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUnLockRotation();

	// Jumping

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnJumpedNetworked();

	void OnJumpedNetworked();

	// Mantling

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	bool TryStartMantlingGrounded();

private:
	bool TryStartMantlingInAir();

	bool IsMantlingAllowedToStart() const;

	bool TryStartMantling(const FAlsMantlingTraceSettings& TraceSettings);

	UFUNCTION(Server, Reliable)
	void ServerStartMantling(const FAlsMantlingParameters& Parameters);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartMantling(const FAlsMantlingParameters& Parameters);

	void StartMantlingImplementation(const FAlsMantlingParameters& Parameters);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	UAlsMantlingSettings* SelectMantlingSettings(EAlsMantlingType MantlingType);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnMantlingStarted(const FAlsMantlingParameters& Parameters);

private:
	void RefreshMantling();

	void StopMantling();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnMantlingEnded();

	// Ragdolling

private:
	bool IsRagdollingAllowedToStart() const;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void StartRagdolling();

private:
	UFUNCTION(Server, Reliable)
	void ServerStartRagdolling();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartRagdolling();

	void StartRagdollingImplementation();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnRagdollingStarted();

private:
	bool IsRagdollingAllowedToStop() const;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	bool TryStopRagdolling();

private:
	UFUNCTION(Server, Reliable)
	void ServerStopRagdolling();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopRagdolling();

	void StopRagdollingImplementation();

public:
	void FinalizeRagdolling();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	UAnimMontage* SelectGetUpMontage(bool bRagdollFacedUpward);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnRagdollingEnded();

private:
	void SetRagdollTargetLocation(const FVector& NewLocation);

	UFUNCTION(Server, Unreliable)
	void ServerSetRagdollTargetLocation(const FVector_NetQuantize100& NewLocation);

	void RefreshRagdolling(float DeltaTime);

	void RefreshRagdollingActorTransform(float DeltaTime);

	// Rolling

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void TryStartRolling(float PlayRate = 1.0f);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	UAnimMontage* SelectRollMontage();

private:
	bool IsRollingAllowedToStart(const UAnimMontage* Montage) const;

	void StartRolling(float PlayRate, float TargetYawAngle);

	UFUNCTION(Server, Reliable)
	void ServerStartRolling(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartRolling(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void StartRollingImplementation(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void RefreshRollingPhysics(float DeltaTime);

	// Debug

public:
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused,
	                          float& VerticalLocation) override;

private:
	static void DisplayDebugHeader(const UCanvas* Canvas, const FText& HeaderText, const FLinearColor& HeaderColor,
	                               float Scale, float HorizontalLocation, float& VerticalLocation);

	void DisplayDebugCurves(const UCanvas* Canvas, float Scale, float HorizontalLocation,
	                        float& VerticalLocation) const;

	void DisplayDebugState(const UCanvas* Canvas, float Scale, float HorizontalLocation, float& VerticalLocation) const;

	void DisplayDebugShapes(const UCanvas* Canvas, float Scale, float HorizontalLocation,
	                        float& VerticalLocation) const;

	void DisplayDebugTraces(const UCanvas* Canvas, float Scale, float HorizontalLocation,
	                        float& VerticalLocation) const;

	void DisplayDebugMantling(const UCanvas* Canvas, float Scale, float HorizontalLocation,
	                          float& VerticalLocation) const;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCharacterMovementComponent> AlsCharacterMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCharacterSettings> Settings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsMovementSettings> MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated,
		Meta = (AllowPrivateAccess))
	EAlsStance DesiredStance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated,
		Meta = (AllowPrivateAccess))
	EAlsGait DesiredGait {EAlsGait::Running};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State",
		ReplicatedUsing = "OnReplicate_DesiredAiming", Meta = (AllowPrivateAccess))
	bool bDesiredAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated,
		Meta = (AllowPrivateAccess))
	EAlsRotationMode DesiredRotationMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated,
		Meta = (AllowPrivateAccess))
	EAlsViewMode ViewMode {EAlsViewMode::ThirdPerson};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State",
		ReplicatedUsing = "OnReplicate_OverlayMode", Meta = (AllowPrivateAccess))
	FGameplayTag OverlayMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character",
		Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UAlsAnimationInstance> AlsAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		ReplicatedUsing = "OnReplicate_FlightMode", Meta = (AllowPrivateAccess))
	EAlsFlightMode FlightMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		Meta = (AllowPrivateAccess))
	EAlsStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		Meta = (AllowPrivateAccess))
	EAlsGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		Meta = (AllowPrivateAccess))
	EAlsRotationMode RotationMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionAction;

	// Raw replicated view rotation. For smooth rotation use FAlsViewState::Rotation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
			  ReplicatedUsing = "OnReplicate_ViewRotation", Meta = (AllowPrivateAccess))
	FRotator ViewRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsViewState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector_NetQuantizeNormal InputDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsLocomotionState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	int32 MantlingRootMotionSourceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated,
		Meta = (AllowPrivateAccess))
	FVector_NetQuantize100 RagdollTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsRagdollingState RagdollingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsRollingState RollingState;

	FTimerHandle BrakingFrictionFactorResetTimer;
};

inline void AAlsCharacter::RefreshTargetYawAngleUsingLocomotionRotation()
{
	RefreshTargetYawAngle(UE_REAL_TO_FLOAT(LocomotionState.Rotation.Yaw));
}

inline void AAlsCharacter::RefreshTargetYawAngle(const float TargetYawAngle)
{
	LocomotionState.TargetYawAngle = TargetYawAngle;

	RefreshViewRelativeTargetYawAngle();

	LocomotionState.SmoothTargetYawAngle = TargetYawAngle;
}

inline void AAlsCharacter::RefreshViewRelativeTargetYawAngle()
{
	LocomotionState.ViewRelativeTargetYawAngle = FRotator3f::NormalizeAxis(
		UE_REAL_TO_FLOAT(ViewState.Rotation.Yaw) - LocomotionState.TargetYawAngle);
}