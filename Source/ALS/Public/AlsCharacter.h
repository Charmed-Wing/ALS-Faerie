#pragma once

#include "GameFramework/Character.h"
#include "State/AlsLocomotionState.h"
#include "State/AlsMantlingState.h"
#include "State/AlsMovementBaseState.h"
#include "State/AlsRagdollingState.h"
#include "State/AlsRollingState.h"
#include "State/AlsViewState.h"
#include "State/AlsFlightState.h"
#include "Utility/AlsGameplayTags.h"
#include "AlsCharacter.generated.h"

enum class EAlsMantlingType : uint8;
struct FAlsMantlingParameters;
struct FAlsMantlingTraceSettings;
class UAlsCharacterMovementComponent;
class UAlsCharacterSettings;
class UAlsMovementSettings;
class UAlsAnimationInstance;
class UAlsMantlingSettings;

class AAlsCharacter;
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FAlsOnDesiredAimingChanged, AAlsCharacter, OnDesiredAimingChangedCallback, AAlsCharacter*, Character, bool, Previous);


UCLASS(AutoExpandCategories = ("Settings|Als Character", "Settings|Als Character|Desired State", "State|Als Character"))
class ALS_API AAlsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AAlsCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual bool CanEditChange(const FProperty* Property) const override;
#endif

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreRegisterAllComponents() override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostNetReceiveLocationAndRotation() override;
	virtual void OnRep_ReplicatedBasedMovement() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Restart() override;

	virtual void Jump() override;
	virtual void OnJumped_Implementation() override;

	virtual bool CanCrouch() const override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual FRotator GetViewRotation() const override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override final;

private:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
						   FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode = 0) override;

	void RefreshMeshProperties() const;

	void RefreshMovementBase();

	void RefreshInput(float DeltaTime);

public:
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
	void SetStance(const FGameplayTag& NewStance);
	void SetGait(const FGameplayTag& NewGait);
	void RefreshGait();
	FGameplayTag CalculateMaxAllowedGait() const;
	FGameplayTag CalculateActualGait(const FGameplayTag& MaxAllowedGait) const;
	bool CanSprint() const;
	void SetRotationMode(const FGameplayTag& NewRotationMode);
	void RefreshRotationMode();
	void SetLocomotionMode(const FGameplayTag& NewLocomotionMode);
	void NotifyLocomotionModeChanged(const FGameplayTag& PreviousLocomotionMode);
	void NotifyLocomotionActionChanged(const FGameplayTag& PreviousLocomotionAction);
	void SetReplicatedViewRotation(const FRotator& NewViewRotation, bool bSendRpc);
	void SetInputDirection(FVector NewInputDirection);
	void SetDesiredVelocityYawAngle(float NewDesiredVelocityYawAngle);

	void SetViewMode(const FGameplayTag& NewViewMode, bool bSendRpc);
	void SetDesiredAiming(bool bNewDesiredAiming, bool bSendRpc);
	void SetDesiredRotationMode(const FGameplayTag& NewDesiredRotationMode, bool bSendRpc);
	void SetDesiredStance(const FGameplayTag& NewDesiredStance, bool bSendRpc);
	void SetDesiredGait(const FGameplayTag& NewDesiredGait, bool bSendRpc);
	void SetOverlayMode(const FGameplayTag& NewOverlayMode, bool bSendRpc);

public:
	void SetLocomotionAction(const FGameplayTag& NewLocomotionAction);

	UFUNCTION(BlueprintCallable, Category = "ALS|Character", Meta = (AutoCreateRefTerm = "NewDesiredStance"))
	void SetDesiredStance(const FGameplayTag& NewDesiredStance);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDesiredGait"))
	void SetDesiredGait(const FGameplayTag& NewDesiredGait);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void SetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewDesiredRotationMode"))
	void SetDesiredRotationMode(const FGameplayTag& NewDesiredRotationMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewViewMode"))
	void SetViewMode(const FGameplayTag& NewViewMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFlightMode"))
	void SetFlightMode(const FGameplayTag& NewFlightMode);

	UFUNCTION(BlueprintCallable, Category = "ALS|Character", Meta = (AutoCreateRefTerm = "NewOverlayMode"))
	void SetOverlayMode(const FGameplayTag& NewOverlayMode);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnLocomotionActionChanged(const FGameplayTag& PreviousLocomotionAction);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnLocomotionModeChanged(const FGameplayTag& PreviousLocomotionMode);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnStanceChanged(const FGameplayTag& PreviousStance);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnGaitChanged(const FGameplayTag& PreviousGait);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnDesiredAimingChanged(bool bPreviousDesiredAiming);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnRotationModeChanged(const FGameplayTag& PreviousRotationMode);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFlightModeChanged(const FGameplayTag& PreviousModeTag);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnOverlayModeChanged(const FGameplayTag& PreviousOverlayMode);

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDesiredStance(const FGameplayTag& NewDesiredStance);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredGait(const FGameplayTag& NewDesiredGait);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetViewMode(const FGameplayTag& NewViewMode);

	UFUNCTION(Server, Reliable)
	void ServerSetFlightMode(const FGameplayTag& NewFlightMode);

	UFUNCTION(Server, Reliable)
	void ServerSetDesiredRotationMode(const FGameplayTag& NewDesiredRotationMode);

	UFUNCTION(Server, Reliable)
	void ServerSetOverlayMode(const FGameplayTag& NewOverlayMode);

	UFUNCTION(Server, Unreliable)
	void ServerSetReplicatedViewRotation(const FRotator& NewViewRotation);

	UFUNCTION(Client, Reliable)
	void ClientSetViewMode(const FGameplayTag& NewViewMode);

	UFUNCTION(Client, Reliable)
	void ClientSetDesiredAiming(bool bNewDesiredAiming);

	UFUNCTION(Client, Reliable)
	void ClientSetDesiredRotationMode(const FGameplayTag& NewDesiredRotationMode);

	UFUNCTION(Client, Reliable)
	void ClientSetDesiredStance(const FGameplayTag& NewDesiredStance);

	UFUNCTION(Client, Reliable)
	void ClientSetDesiredGait(const FGameplayTag& NewDesiredGait);

	UFUNCTION(Client, Reliable)
	void ClientSetOverlayMode(const FGameplayTag& NewOverlayMode);

private:
	UFUNCTION()
	void OnReplicate_FlightMode(const FGameplayTag& PreviousFlightMode);

	UFUNCTION()
	void OnReplicated_DesiredAiming(bool bPreviousDesiredAiming);

	UFUNCTION()
	void OnReplicated_OverlayMode(const FGameplayTag& PreviousOverlayMode);

	UFUNCTION()
	void OnReplicated_ReplicatedViewRotation();

public:
	void CorrectViewNetworkSmoothing(const FRotator& NewTargetRotation, bool bRelativeTargetRotation);

private:
	void RefreshView(float DeltaTime);

	void RefreshViewNetworkSmoothing(float DeltaTime);

	void RefreshLocomotionLocationAndRotation();

	void RefreshLocomotionEarly();

	void RefreshLocomotion(float DeltaTime);

	void RefreshLocomotionLate(float DeltaTime);


	/************************/
	/*		Rotation		*/
	/************************/
public:
	void CharacterMovement_OnPhysicsRotation(float DeltaTime);

private:
	void RefreshGroundedRotation(float DeltaTime);
	void RefreshFallingRotation(float DeltaTime);
	void RefreshFlyingRotation(float DeltaTime);
	void RefreshSwimmingRotation(float DeltaTime);

	void ApplyRotationYawSpeedAnimationCurve(float DeltaTime);

protected:
	virtual bool RefreshCustomGroundedMovingRotation(float DeltaTime);
	virtual bool RefreshCustomGroundedNotMovingRotation(float DeltaTime);

	virtual bool RefreshCustomFallingRotation(float DeltaTime);
	virtual bool RefreshCustomFlyingMovingRotation(float DeltaTime);
	virtual bool RefreshCustomFlyingNotMovingRotation(float DeltaTime);

	void RefreshGroundedAimingRotation(float DeltaTime);
	bool RefreshConstrainedAimingRotation(float DeltaTime, const bool bApplySecondaryConstraint = false);

	void RefreshFlyingMovingAimingRotation(float DeltaTime);
	void RefreshFlyingNotMovingAimingRotation(float DeltaTime);

	void RefreshFallingAimingRotation(float DeltaTime);

	float CalculateGroundedMovingRotationInterpolationSpeed() const;

	void RefreshRotation(float TargetYawAngle, float DeltaTime, float RotationInterpolationSpeed);

	void RefreshRotationExtraSmooth(float TargetYawAngle, float DeltaTime,
	                                float RotationInterpolationSpeed, float TargetYawAngleRotationSpeed);

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
	// Can this character start flying currently. Performs a FlightCheck to see if conditions allow for it.
	bool CanFly() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	bool FlightCheck() const;

	// Determine if a HitResult should trigger a flight interrupt
	UFUNCTION(BlueprintNativeEvent)
	bool FlightInterruptCheck(const FHitResult& Hit);

	float FlightTrace(float Distance, const FVector& Direction);


	/************************/
	/*		Rolling			*/
	/************************/
public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Character")
	void StartRolling(float PlayRate = 1.0f);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UAnimMontage* SelectRollMontage();

	bool IsRollingAllowedToStart(const UAnimMontage* Montage) const;

private:
	void StartRolling(float PlayRate, float TargetYawAngle);

	UFUNCTION(Server, Reliable)
	void ServerStartRolling(UAnimMontage* Montage, float PlayRate, float InitialYawAngle, float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartRolling(UAnimMontage* Montage, float PlayRate, float InitialYawAngle, float TargetYawAngle);

	void StartRollingImplementation(UAnimMontage* Montage, float PlayRate, float InitialYawAngle, float TargetYawAngle);

	void RefreshRolling(float DeltaTime);

	void RefreshRollingPhysics(float DeltaTime);


	/************************/
	/*		Mantling		*/
	/************************/
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	bool IsMantlingAllowedToStart(const FAlsMantlingParameters& Parameters) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Character", Meta = (ReturnDisplayName = "Success"))
	bool StartMantlingGrounded();

private:
	bool StartMantlingInAir();

	bool StartMantling(const FAlsMantlingTraceSettings& TraceSettings);

	UFUNCTION(Server, Reliable)
	void ServerStartMantling(const FAlsMantlingParameters& Parameters);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartMantling(const FAlsMantlingParameters& Parameters);

	void StartMantlingImplementation(const FAlsMantlingParameters& Parameters);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UAlsMantlingSettings* SelectMantlingSettings(EAlsMantlingType MantlingType);

	float CalculateMantlingStartTime(const UAlsMantlingSettings* MantlingSettings, float MantlingHeight) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnMantlingStarted(const FAlsMantlingParameters& Parameters);

private:
	void RefreshMantling();

	void StopMantling(bool bStopMontage = false);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnMantlingEnded();


	/************************/
	/*		Ragdolling		*/
	/************************/
private:
	bool IsRagdollingAllowedToStart() const;

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Character")
	void StartRagdolling();

private:
	UFUNCTION(Server, Reliable)
	void ServerStartRagdolling();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartRagdolling();

	void StartRagdollingImplementation();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnRagdollingStarted();

public:
	bool IsRagdollingAllowedToStop() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Character", Meta = (ReturnDisplayName = "Success"))
	bool StopRagdolling();

private:
	UFUNCTION(Server, Reliable)
	void ServerStopRagdolling();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopRagdolling();

	void StopRagdollingImplementation();

public:
	void FinalizeRagdolling();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UAnimMontage* SelectGetUpMontage(bool bRagdollFacedUpward);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnRagdollingEnded();

private:
	void SetRagdollTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Unreliable)
	void ServerSetRagdollTargetLocation(const FVector_NetQuantize100& NewTargetLocation);

	void RefreshRagdolling(float DeltaTime);

	void RefreshRagdollingActorTransform(float DeltaTime);


	/************************/
	/*		Debugging		*/
	/************************/
public:
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;

private:
	static void DisplayDebugHeader(const UCanvas* Canvas, const FText& HeaderText, const FLinearColor& HeaderColor,
	                               float Scale, float HorizontalLocation, float& VerticalLocation);

	void DisplayDebugCurves(const UCanvas* Canvas, float Scale, float HorizontalLocation, float& VerticalLocation) const;

	void DisplayDebugState(const UCanvas* Canvas, float Scale, float HorizontalLocation, float& VerticalLocation) const;

	void DisplayDebugShapes(const UCanvas* Canvas, float Scale, float HorizontalLocation, float& VerticalLocation) const;

	void DisplayDebugTraces(const UCanvas* Canvas, float Scale, float HorizontalLocation, float& VerticalLocation) const;

	void DisplayDebugMantling(const UCanvas* Canvas, float Scale, float HorizontalLocation, float& VerticalLocation) const;


	/************************/
	/*		Delegates		*/
	/************************/
public:
	UPROPERTY(BlueprintAssignable, Category = "Callbacks", meta = (DisplayName = "On Desired Aiming Changed"))
	FAlsOnDesiredAimingChanged OnDesiredAimingChangedCallback;


	/************************/
	/*		Variables		*/
	/************************/

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Als Character")
	TObjectPtr<UAlsCharacterMovementComponent> AlsCharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")
	TObjectPtr<UAlsCharacterSettings> Settings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")
	TObjectPtr<UAlsMovementSettings> MovementSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State",
		ReplicatedUsing = "OnReplicated_DesiredAiming")
	bool bDesiredAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag DesiredRotationMode{AlsRotationModeTags::ViewDirection};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag DesiredStance{AlsStanceTags::Standing};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag DesiredGait{AlsGaitTags::Running};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State", Replicated)
	FGameplayTag ViewMode{AlsViewModeTags::ThirdPerson};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character|Desired State",
		ReplicatedUsing = "OnReplicated_OverlayMode")
	FGameplayTag OverlayMode{AlsOverlayModeTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (ShowInnerProperties))
	TWeakObjectPtr<UAlsAnimationInstance> AnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FGameplayTag LocomotionMode{AlsLocomotionModeTags::Grounded};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		ReplicatedUsing = "OnReplicate_FlightMode")
	FGameplayTag FlightMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FGameplayTag RotationMode{AlsRotationModeTags::ViewDirection};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FGameplayTag Stance{AlsStanceTags::Standing};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FGameplayTag Gait{AlsGaitTags::Walking};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FGameplayTag LocomotionAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsMovementBaseState MovementBase;

	// Replicated raw view rotation. Depending on the context, this rotation can be in world space, or in movement
	// base space. In most cases, it is better to use FAlsViewState::Rotation to take advantage of network smoothing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient,
		ReplicatedUsing = "OnReplicated_ReplicatedViewRotation")
	FRotator ReplicatedViewRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsViewState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated)
	FVector_NetQuantizeNormal InputDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsFlightState FlightState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character",
		Transient, Replicated, Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
	float DesiredVelocityYawAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsLocomotionState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsMantlingState MantlingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated)
	FVector_NetQuantize100 RagdollTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsRagdollingState RagdollingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)
	FAlsRollingState RollingState;

	FTimerHandle BrakingFrictionFactorResetTimer;
};