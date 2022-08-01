#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Settings/AlsMantlingSettings.h"
#include "State/AlsLocomotionState.h"
#include "State/AlsRagdollingState.h"
#include "State/AlsRollingState.h"
#include "State/AlsViewState.h"
#include "Utility/AlsGameplayTags.h"
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
	virtual void PreRegisterAllComponents() override;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void PostNetReceiveLocationAndRotation() override;

	virtual void OnRep_ReplicatedBasedMovement() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Restart() override;

	virtual void Jump() override;
	virtual void OnJumped_Implementation() override;

	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual FRotator GetViewRotation() const override;
	virtual void FaceRotation(FRotator NewRotation, float DeltaTime) override final;

private:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
						   FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMode, uint8 PreviousCustomMode = 0) override;

public:
	bool IsSimulatedProxyTeleported() const { return bSimulatedProxyTeleported; }
	const FGameplayTag& GetDesiredStance() const			{ return DesiredStance; }
	const FGameplayTag& GetStance() const					{ return Stance; }
	const FGameplayTag& GetDesiredGait() const				{ return DesiredGait; }
	const FGameplayTag& GetGait() const						{ return Gait; }
	bool IsDesiredAiming() const							{ return bDesiredAiming; }
	const FGameplayTag& GetDesiredRotationMode() const		{ return DesiredRotationMode; }
	const FGameplayTag& GetRotationMode() const				{ return RotationMode; }
	const FGameplayTag& GetViewMode() const					{ return ViewMode; }
	const FGameplayTag& GetFlightMode() const				{ return FlightMode; }
	const FGameplayTag& GetLocomotionMode() const			{ return LocomotionMode; }
	const FGameplayTag& GetLocomotionAction() const			{ return LocomotionAction; }
	const FGameplayTag& GetOverlayMode() const				{ return OverlayMode; }
	const FVector& GetInputDirection() const				{ return InputDirection; }
	const FAlsLocomotionState& GetLocomotionState() const	{ return LocomotionState; }
	const FAlsViewState& GetViewState() const				{ return ViewState; }

	bool IsInAir() const;

private:
	void ApplyDesiredStance();
	void SetStance(const FGameplayTag& NewStanceTag);
	void SetGait(const FGameplayTag& NewGaitTag);
	void SetViewRotation(const FRotator& NewViewRotation);
	void RefreshGait();
	FGameplayTag CalculateMaxAllowedGait() const;
	FGameplayTag CalculateActualGait(const FGameplayTag& MaxAllowedGaitTag) const;
	bool CanSprint() const;
	void SetRotationMode(const FGameplayTag& NewModeTag);
	void RefreshRotationMode();
	void SetLocomotionMode(const FGameplayTag& NewModeTag);
	void NotifyLocomotionModeChanged(const FGameplayTag& PreviousModeTag);
	void RefreshVisibilityBasedAnimTickOption() const;

public:
	void SetLocomotionAction(const FGameplayTag& NewActionTag);
	void NotifyLocomotionActionChanged(const FGameplayTag& PreviousActionTag);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewStanceTag"))
	void SetDesiredStance(const FGameplayTag& NewStanceTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewGaitTag"))
	void SetDesiredGait(const FGameplayTag& NewGaitTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewModeTag"))
	void SetDesiredRotationMode(const FGameplayTag& NewModeTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewModeTag"))
	void SetViewMode(const FGameplayTag& NewModeTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewModeTag"))
	void SetFlightMode(const FGameplayTag& NewModeTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewModeTag"))
	void SetOverlayMode(const FGameplayTag& NewModeTag);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnLocomotionActionChanged(const FGameplayTag& PreviousActionTag);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnLocomotionModeChanged(const FGameplayTag& PreviousModeTag);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnStanceChanged(const FGameplayTag& PreviousStance);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnGaitChanged(const FGameplayTag& PreviousGaitTag);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnDesiredAimingChanged(bool bPreviousDesiredAiming);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnRotationModeChanged(const FGameplayTag& PreviousModeTag);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFlightModeChanged(const FGameplayTag& PreviousModeTag);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnOverlayModeChanged(const FGameplayTag& PreviousModeTag);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredStance(const FGameplayTag& NewStanceTag);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGait(const FGameplayTag& NewGaitTag);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetViewMode(const FGameplayTag& NewModeTag);

	UFUNCTION(Server, Reliable)
	void ServerSetFlightMode(const FGameplayTag& NewModeTag);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredRotationMode(const FGameplayTag& NewModeTag);

	UFUNCTION(Server, Reliable)
	void ServerSetOverlayMode(const FGameplayTag& NewModeTag);

	UFUNCTION(Server, Unreliable)
	void ServerSetViewRotation(const FRotator& NewViewRotation);

private:
	UFUNCTION()
	void OnReplicate_DesiredAiming(bool bPreviousDesiredAiming);

	UFUNCTION()
	void OnReplicate_FlightMode(const FGameplayTag& PreviousModeTag);

	UFUNCTION()
	void OnReplicated_DesiredAiming(bool bPreviousDesiredAiming);

	UFUNCTION()
	void OnReplicated_OverlayMode(const FGameplayTag& PreviousModeTag);

	UFUNCTION()
	void OnReplicated_ViewRotation();

public:
	void CorrectViewNetworkSmoothing(const FRotator& NewViewRotation);

private:
	void RefreshView(float DeltaTime);

	void RefreshViewNetworkSmoothing(float DeltaTime);

	// Locomotion

private:
	void SetInputDirection(FVector NewInputDirection);

	void RefreshLocomotionLocationAndRotation(float DeltaTime);

	void RefreshLocomotion(float DeltaTime);


	/************************/
	/*		Rotation		*/
	/************************/
public:
	void PhysicsRotation(float DeltaTime);

private:
	void ApplyRotationYawSpeed(float DeltaTime);

	void RefreshGroundedRotation(float DeltaTime);
	void RefreshFallingRotation(float DeltaTime);
	void RefreshFlyingRotation(float DeltaTime);
	void RefreshSwimmingRotation(float DeltaTime);

protected:
	// Custom rotation

	virtual bool TryRefreshCustomGroundedMovingRotation(float DeltaTime);
	virtual bool TryRefreshCustomGroundedNotMovingRotation(float DeltaTime);

	virtual bool TryRefreshCustomFallingRotation(float DeltaTime);

	virtual bool TryRefreshCustomFlyingMovingRotation(float DeltaTime);
	virtual bool TryRefreshCustomFlyingNotMovingRotation(float DeltaTime);

	virtual bool TryRefreshCustomSwimmingMovingRotation(float DeltaTime);
	virtual bool TryRefreshCustomSwimmingNotMovingRotation(float DeltaTime);


	void RefreshGroundedMovingAimingRotation(float DeltaTime);
	void RefreshGroundedNotMovingAimingRotation(float DeltaTime);

	void RefreshFlyingMovingAimingRotation(float DeltaTime);
	void RefreshFlyingNotMovingAimingRotation(float DeltaTime);

	void RefreshSwimmingMovingAimingRotation(float DeltaTime);
	void RefreshSwimmingNotMovingAimingRotation(float DeltaTime);

	float CalculateRotationInterpolationSpeed() const;


protected:
	void RefreshFallingAimingRotation(float DeltaTime);

	void RefreshRotation(float TargetYawAngle, float DeltaTime, float RotationInterpolationSpeed);

	void RefreshRotationExtraSmooth(float TargetYawAngle, float DeltaTime,
	                                float RotationInterpolationSpeed,
	                                float TargetYawAngleRotationSpeed);

	void RefreshRotationInstant(float TargetYawAngle, ETeleportType Teleport = ETeleportType::None);

	void RefreshTargetYawAngleUsingLocomotionRotation();

	void RefreshTargetYawAngle(float TargetYawAngle);

	void RefreshViewRelativeTargetYawAngle();


	/************************/
	/*		Jumping			*/
	/************************/
private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnJumpedNetworked();

	void OnJumpedNetworked();


	/************************/
	/*		Flying			*/
	/************************/
protected:
	UFUNCTION(BlueprintNativeEvent)
	bool FlightCheck();

	bool CanFly();

	UFUNCTION(BlueprintNativeEvent)
	bool FlightInterruptCheck(AActor* Actor, const FVector& Vector, const FHitResult& Hit);

	float FlightTrace(float Distance, const FVector& Direction);

	/************************/
	/*		Rotation Lock	*/
	/************************/
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


	/************************/
	/*		Rolling			*/
	/************************/
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

	void RefreshRolling(float DeltaTime);

	void RefreshRollingPhysics(float DeltaTime);


	/************************/
	/*		Mantling		*/
	/************************/
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	bool IsMantlingAllowedToStart() const;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	bool TryStartMantlingGrounded();

private:
	bool TryStartMantlingInAir();

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


	/************************/
	/*		Ragdolling		*/
	/************************/
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


	/************************/
	/*		Debugging		*/
	/************************/
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


	/************************/
	/*		Variables		*/
	/************************/

	/* All variables are private and interacted with through c++ getters/setters or BP private access */
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCharacterMovementComponent> AlsCharacterMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCharacterSettings> Settings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings|Als Character", Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsMovementSettings> MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State",
		ReplicatedUsing = "OnReplicated_DesiredAiming", Meta = (AllowPrivateAccess))
	bool bDesiredAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredRotationMode{AlsRotationModeTags::LookingDirection};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredStance{AlsStanceTags::Standing};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag DesiredGait{AlsGaitTags::Running};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated, Meta = (AllowPrivateAccess))
	FGameplayTag ViewMode{AlsViewModeTags::ThirdPerson};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State",
		ReplicatedUsing = "OnReplicated_OverlayMode", Meta = (AllowPrivateAccess))
	FGameplayTag OverlayMode{AlsOverlayModeTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	bool bSimulatedProxyTeleported;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character",
		Transient, Meta = (AllowPrivateAccess, ShowInnerProperties))
	TObjectPtr<UAlsAnimationInstance> AnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionMode{AlsLocomotionModeTags::Grounded};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		ReplicatedUsing = "OnReplicate_FlightMode", Meta = (AllowPrivateAccess))
	FGameplayTag FlightMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag RotationMode{AlsRotationModeTags::LookingDirection};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Stance{AlsStanceTags::Standing};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Gait{AlsGaitTags::Walking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionAction;

	// Raw replicated view rotation. For smooth rotation use FAlsViewState::Rotation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		ReplicatedUsing = "OnReplicated_ViewRotation", Meta = (AllowPrivateAccess))
	FRotator ViewRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsViewState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector_NetQuantizeNormal InputDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsLocomotionState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	int32 MantlingRootMotionSourceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated, Meta = (AllowPrivateAccess))
	FVector_NetQuantize100 RagdollTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsRagdollingState RagdollingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FAlsRollingState RollingState;

	FTimerHandle BrakingFrictionFactorResetTimer;
};