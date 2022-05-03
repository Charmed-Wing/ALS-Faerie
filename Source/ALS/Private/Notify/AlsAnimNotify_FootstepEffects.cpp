#include "Notify/AlsAnimNotify_FootstepEffects.h"

#include "AlsCharacter.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsEnumUtility.h"
#include "Utility/AlsMath.h"
#include "Utility/AlsUtility.h"
#include "Utility/GameplayTags/AlsLocomotionModeTags.h"

FString UAlsAnimNotify_FootstepEffects::GetNotifyName_Implementation() const
{
	return FString::Format(TEXT("Als Footstep Effects: {0}"), {AlsEnumUtility::GetNameStringByValue(FootBone)});
}

void UAlsAnimNotify_FootstepEffects::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(Mesh, Animation, EventReference);

	if (!IsValid(Mesh) || FootstepEffectsSettings.IsNull())
	{
		return;
	}

	const AAlsCharacter* Character {Cast<AAlsCharacter>(Mesh->GetOwner())};

	if (bSkipEffectsWhenInAir && IsValid(Character)
		&& Character->IsInAir())
	{
		return;
	}

	const double CapsuleScale {IsValid(Character) ? Character->GetCapsuleComponent()->GetComponentScale().Z : 1.0f};

	const auto* World{Mesh->GetWorld()};
	const auto* AnimationInstance{Mesh->GetAnimInstance()};

	const FName FootBoneName = FootBone == EAlsFootBone::Left
		                           ? UAlsConstants::FootLeftIkBone()
		                           : UAlsConstants::FootRightIkBone();
	const FTransform FootTransform = Mesh->GetSocketTransform(FootBoneName);

#if ENABLE_DRAW_DEBUG
	const bool bDisplayDebug {UAlsUtility::ShouldDisplayDebug(Mesh->GetOwner(), UAlsConstants::TracesDisplayName())};
#endif

	FCollisionQueryParams QueryParameters{ANSI_TO_TCHAR(__FUNCTION__), true, Mesh->GetOwner()};
	QueryParameters.bReturnPhysicalMaterial = true;

	FHitResult Hit{};
	if (World->LineTraceSingleByChannel(Hit, FootTransform.GetLocation(),
	                                    FootTransform.GetLocation() - FVector{
		                                    0.0f, 0.0f, FootstepEffectsSettings->SurfaceTraceDistance * CapsuleScale
	                                    },
	                                    UEngineTypes::ConvertToCollisionChannel(FootstepEffectsSettings->SurfaceTraceChannel),
	                                    QueryParameters))
	{
#if ENABLE_DRAW_DEBUG
		if (bDisplayDebug)
		{
			UAlsUtility::DrawDebugLineTraceSingle(World, Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit,
			                                      Hit, {0.333333f, 0.0f, 0.0f}, FLinearColor::Red, 10.0f);
		}
#endif
	}
	else
	{
		Hit.ImpactPoint = FootTransform.GetLocation();
		Hit.ImpactNormal = FVector::UpVector;
	}

	const EPhysicalSurface SurfaceType = Hit.PhysMaterial.IsValid() ? Hit.PhysMaterial->SurfaceType.GetValue() : SurfaceType_Default;
	const FAlsFootstepEffectSettings* EffectSettings = nullptr;

	for (const FAlsFootstepEffectSettings& Effect : FootstepEffectsSettings->Effects)
	{
		if (Effect.SurfaceType == SurfaceType)
		{
			EffectSettings = &Effect;
			break;
		}

		if (EffectSettings == nullptr)
		{
			EffectSettings = &Effect;
		}
	}

	if (EffectSettings == nullptr)
	{
		return;
	}

	const auto FootstepLocation{Hit.ImpactPoint};

	const auto FootstepRotation{
		FRotationMatrix::MakeFromZY(Hit.ImpactNormal,
		                            FootTransform.TransformVectorNoScale(FootBone == EAlsFootBone::Left
			                                                                 ? FootstepEffectsSettings->FootLeftYAxis
			                                                                 : FootstepEffectsSettings->FootRightYAxis))
		.Rotator()
	};

#if ENABLE_DRAW_DEBUG
	if (bDisplayDebug)
	{
		DrawDebugCoordinateSystem(World, FootstepLocation, FootstepRotation, 25.0f, false, 10.0f, 0,
		                          UAlsUtility::DrawLineThickness);
	}
#endif

	if (bSpawnSound)
	{
		float VolumeMultiplier {SoundVolumeMultiplier};

		if (!bIgnoreFootstepSoundBlockCurve && IsValid(AnimationInstance))
		{
			VolumeMultiplier *= 1.0f - UAlsMath::Clamp01(
				AnimationInstance->GetCurveValue(UAlsConstants::FootstepSoundBlockCurve()));
		}

		if (FAnimWeight::IsRelevant(VolumeMultiplier) && IsValid(EffectSettings->Sound.LoadSynchronous()))
		{
			UAudioComponent* Audio {nullptr};

			switch (EffectSettings->SoundSpawnType)
			{
			case EAlsFootstepSoundSpawnType::SpawnAtTraceHitLocation:
				if (World->WorldType == EWorldType::EditorPreview)
				{
					UGameplayStatics::PlaySoundAtLocation(World, EffectSettings->Sound.Get(), FootstepLocation,
					                                      VolumeMultiplier, SoundPitchMultiplier);
				}
				else
				{
					Audio = UGameplayStatics::SpawnSoundAtLocation(World, EffectSettings->Sound.Get(), FootstepLocation,
					                                               FootstepRotation, VolumeMultiplier,
					                                               SoundPitchMultiplier);
				}
				break;

			case EAlsFootstepSoundSpawnType::SpawnAttachedToFootBone:
				Audio = UGameplayStatics::SpawnSoundAttached(EffectSettings->Sound.Get(), Mesh, FootBoneName,
				                                             FVector::ZeroVector,
				                                             FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
				                                             true, VolumeMultiplier, SoundPitchMultiplier);
				break;
			}

			if (IsValid(Audio))
			{
				Audio->SetIntParameter(TEXT("FootstepType"), static_cast<int32>(SoundType));
			}
		}
	}

	if (bSpawnDecal && IsValid(EffectSettings->DecalMaterial.LoadSynchronous()))
	{
		const FRotator DecalRotation {
			FootstepRotation + (FootBone == EAlsFootBone::Left
				                    ? EffectSettings->DecalFootLeftRotationOffset
				                    : EffectSettings->DecalFootRightRotationOffset)
		};

		const FVector DecalLocation {
			FootstepLocation + DecalRotation.RotateVector(EffectSettings->DecalLocationOffset * CapsuleScale)
		};

		UDecalComponent* Decal;

		if (EffectSettings->DecalSpawnType == EAlsFootstepDecalSpawnType::SpawnAttachedToTraceHitComponent && Hit.Component.IsValid())
		{
			Decal = UGameplayStatics::SpawnDecalAttached(EffectSettings->DecalMaterial.Get(), EffectSettings->DecalSize * CapsuleScale,
			                                             Hit.Component.Get(), NAME_None, DecalLocation,
			                                             DecalRotation, EAttachLocation::KeepWorldPosition);
		}
		else
		{
			Decal = UGameplayStatics::SpawnDecalAtLocation(World, EffectSettings->DecalMaterial.Get(),
			                                               EffectSettings->DecalSize * CapsuleScale, DecalLocation,
			                                               DecalRotation);
		}

		if (IsValid(Decal))
		{
			Decal->SetFadeOut(EffectSettings->DecalDuration, EffectSettings->DecalFadeOutDuration, false);
		}
	}

	if (bSpawnParticleSystem && IsValid(EffectSettings->ParticleSystem.LoadSynchronous()))
	{
		switch (EffectSettings->ParticleSystemSpawnType)
		{
		case EAlsFootstepParticleEffectSpawnType::SpawnAtTraceHitLocation:
			{
				const FRotator ParticleSystemRotation {
					FootstepRotation + (FootBone == EAlsFootBone::Left
						                    ? EffectSettings->ParticleSystemFootLeftRotationOffset
						                    : EffectSettings->ParticleSystemFootLeftRotationOffset)
				};

				const FVector ParticleSystemLocation {
					FootstepLocation + ParticleSystemRotation.RotateVector(
						EffectSettings->ParticleSystemLocationOffset * CapsuleScale)
				};

				UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, EffectSettings->ParticleSystem.Get(),
				                                               ParticleSystemLocation, ParticleSystemRotation,
				                                               FVector::OneVector * CapsuleScale, true, true,
				                                               ENCPoolMethod::AutoRelease);
			}
			break;

		case EAlsFootstepParticleEffectSpawnType::SpawnAttachedToFootBone:
			UNiagaraFunctionLibrary::SpawnSystemAttached(EffectSettings->ParticleSystem.Get(), Mesh, FootBoneName,
			                                             EffectSettings->ParticleSystemLocationOffset * CapsuleScale,
			                                             EffectSettings->ParticleSystemFootLeftRotationOffset,
			                                             FVector::OneVector * CapsuleScale,
			                                             EAttachLocation::KeepRelativeOffset,
			                                             true, ENCPoolMethod::AutoRelease);
			break;
		}
	}
}
