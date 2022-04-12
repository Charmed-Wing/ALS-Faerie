#include "AlsCharacter.h"

#include "DisplayDebugHelpers.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Canvas.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsEnumUtility.h"
#include "Utility/AlsMath.h"
#include "Utility/AlsUtility.h"

void AAlsCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused,
                                 float& VerticalLocation)
{
	const float Scale {
		FMath::Min(Canvas->SizeX / (1280.0f * Canvas->GetDPIScale()), Canvas->SizeY / (720.0f * Canvas->GetDPIScale()))
	};

	const float RowOffset {12.0f * Scale};
	const float ColumnOffset {200.0f * Scale};

	float MaxVerticalLocation {VerticalLocation};
	float HorizontalLocation {5.0f * Scale};

	static const FText DebugModeHeaderText {
		FText::AsCultureInvariant(TEXT("Debug mode is enabled! Press (Shift + 0) to disable."))
	};

	DisplayDebugHeader(Canvas, DebugModeHeaderText, FLinearColor::Green, Scale, HorizontalLocation, VerticalLocation);

	VerticalLocation += RowOffset;
	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	if (!DebugDisplay.IsDisplayOn(UAlsConstants::CurvesDisplayName()) && !DebugDisplay.IsDisplayOn(
			UAlsConstants::StateDisplayName()) &&
		!DebugDisplay.IsDisplayOn(UAlsConstants::ShapesDisplayName()) && !DebugDisplay.IsDisplayOn(
			UAlsConstants::TracesDisplayName()) &&
		!DebugDisplay.IsDisplayOn(UAlsConstants::MantlingDisplayName()))
	{
		VerticalLocation = MaxVerticalLocation;

		Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalLocation);
		return;
	}

	const float InitialVerticalLocation {VerticalLocation};

	static const FText CurvesHeaderText {FText::AsCultureInvariant(TEXT("ALS.Curves (Shift + 1)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::CurvesDisplayName()))
	{
		DisplayDebugHeader(Canvas, CurvesHeaderText, FLinearColor::Green, Scale, HorizontalLocation, VerticalLocation);
		DisplayDebugCurves(Canvas, Scale, HorizontalLocation, VerticalLocation);

		MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation + RowOffset);
		VerticalLocation = InitialVerticalLocation;
		HorizontalLocation += ColumnOffset;
	}
	else
	{
		DisplayDebugHeader(Canvas, CurvesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalLocation,
		                   VerticalLocation);

		VerticalLocation += RowOffset;
	}

	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	static const FText StateHeaderText {FText::AsCultureInvariant(TEXT("ALS.State (Shift + 2)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::StateDisplayName()))
	{
		DisplayDebugHeader(Canvas, StateHeaderText, FLinearColor::Green, Scale, HorizontalLocation, VerticalLocation);
		DisplayDebugState(Canvas, Scale, HorizontalLocation, VerticalLocation);
	}
	else
	{
		DisplayDebugHeader(Canvas, StateHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalLocation,
		                   VerticalLocation);
	}

	VerticalLocation += RowOffset;
	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	static const FText ShapesHeaderText {FText::AsCultureInvariant(TEXT("ALS.Shapes (Shift + 3)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::ShapesDisplayName()))
	{
		DisplayDebugHeader(Canvas, ShapesHeaderText, FLinearColor::Green, Scale, HorizontalLocation, VerticalLocation);
		DisplayDebugShapes(Canvas, Scale, HorizontalLocation, VerticalLocation);
	}
	else
	{
		DisplayDebugHeader(Canvas, ShapesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalLocation,
		                   VerticalLocation);
	}

	VerticalLocation += RowOffset;
	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	static const FText TracesHeaderText {FText::AsCultureInvariant(TEXT("ALS.Traces (Shift + 4)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::TracesDisplayName()))
	{
		DisplayDebugHeader(Canvas, TracesHeaderText, FLinearColor::Green, Scale, HorizontalLocation, VerticalLocation);
		DisplayDebugTraces(Canvas, Scale, HorizontalLocation, VerticalLocation);
	}
	else
	{
		DisplayDebugHeader(Canvas, TracesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalLocation,
		                   VerticalLocation);
	}

	VerticalLocation += RowOffset;
	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	static const FText MantlingHeaderText {FText::AsCultureInvariant(TEXT("ALS.Mantling (Shift + 5)"))};

	if (DebugDisplay.IsDisplayOn(UAlsConstants::MantlingDisplayName()))
	{
		DisplayDebugHeader(Canvas, MantlingHeaderText, FLinearColor::Green, Scale, HorizontalLocation,
		                   VerticalLocation);
		DisplayDebugMantling(Canvas, Scale, HorizontalLocation, VerticalLocation);
	}
	else
	{
		DisplayDebugHeader(Canvas, MantlingHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalLocation,
		                   VerticalLocation);
	}

	VerticalLocation += RowOffset;
	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	VerticalLocation = MaxVerticalLocation;

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalLocation);
}

void AAlsCharacter::DisplayDebugHeader(const UCanvas* Canvas, const FText& HeaderText, const FLinearColor& HeaderColor,
                                       const float Scale, const float HorizontalLocation, float& VerticalLocation)
{
	FCanvasTextItem Text {
		{HorizontalLocation, VerticalLocation},
		HeaderText,
		GEngine->GetMediumFont(),
		HeaderColor
	};

	Text.Scale = {Scale, Scale};
	Text.EnableShadow(FLinearColor::Black);

	Text.Draw(Canvas->Canvas);

	VerticalLocation += 15.0f * Scale;
}

void AAlsCharacter::DisplayDebugCurves(const UCanvas* Canvas, const float Scale,
                                       const float HorizontalLocation, float& VerticalLocation) const
{
	VerticalLocation += 4.0f * Scale;

	FCanvasTextItem CanvasTextItem {
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	CanvasTextItem.Scale = {Scale * 0.75f, Scale * 0.75f};
	CanvasTextItem.EnableShadow(FLinearColor::Black);

	const float RowOffset {12.0f * Scale};
	const float ColumnOffset {145.0f * Scale};

	static TArray<FName> CurveNames;
	check(CurveNames.Num() <= 0)

	GetMesh()->GetAnimInstance()->GetAllCurveNames(CurveNames);

	CurveNames.Sort([](const FName& A, const FName& B) { return A.LexicalLess(B); });

	for (const FName& CurveName : CurveNames)
	{
		const float CurveValue {GetMesh()->GetAnimInstance()->GetCurveValue(CurveName)};

		CanvasTextItem.SetColor(FMath::Lerp(FLinearColor::Gray, FLinearColor::White, UAlsMath::Clamp01(CurveValue)));

		CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(CurveName.ToString(), false));
		CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

		CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), CurveValue));
		CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

		VerticalLocation += RowOffset;
	}

	CurveNames.Reset();
}

void AAlsCharacter::DisplayDebugState(const UCanvas* Canvas, const float Scale,
                                      const float HorizontalLocation, float& VerticalLocation) const
{
	VerticalLocation += 4.0f * Scale;

	FCanvasTextItem CanvasTextItem {
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	CanvasTextItem.Scale = {Scale * 0.75f, Scale * 0.75f};
	CanvasTextItem.EnableShadow(FLinearColor::Black);

	const float RowOffset {12.0f * Scale};
	const float ColumnOffset {120.0f * Scale};

	static const FText DesiredStanceText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, DesiredStance), false))
	};

	CanvasTextItem.Text = DesiredStanceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(DesiredStance), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText StanceText {
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, Stance), false))
	};

	CanvasTextItem.Text = StanceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(Stance), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText DesiredGaitText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, DesiredGait), false))
	};

	CanvasTextItem.Text = DesiredGaitText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(DesiredGait), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText GaitText {
		FText::AsCultureInvariant(FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, Gait), false))
	};

	CanvasTextItem.Text = GaitText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(Gait), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText DesiredAimingText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, bDesiredAiming), true))
	};

	CanvasTextItem.Text = DesiredAimingText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(UAlsUtility::BoolToString(bDesiredAiming), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText DesiredRotationModeText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, DesiredRotationMode), false))
	};

	CanvasTextItem.Text = DesiredRotationModeText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(
		FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(DesiredRotationMode), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText RotationModeText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, RotationMode), false))
	};

	CanvasTextItem.Text = RotationModeText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(RotationMode), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText ViewModeText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, ViewMode), false))
	};

	CanvasTextItem.Text = ViewModeText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FName::NameToDisplayString(AlsEnumUtility::GetNameStringByValue(ViewMode), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText OverlayModeText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, OverlayMode), false))
	};

	CanvasTextItem.Text = OverlayModeText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(
		FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(OverlayMode).ToString(), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText LocomotionModeText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, LocomotionMode), false))
	};

	CanvasTextItem.Text = LocomotionModeText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(
		FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(LocomotionMode).ToString(), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText LocomotionActionText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, LocomotionAction), false))
	};

	CanvasTextItem.Text = LocomotionActionText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(
		FName::NameToDisplayString(UAlsUtility::GetSimpleTagName(LocomotionAction).ToString(), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;
}

void AAlsCharacter::DisplayDebugShapes(const UCanvas* Canvas, const float Scale,
                                       const float HorizontalLocation, float& VerticalLocation) const
{
	VerticalLocation += 4.0f * Scale;

	FCanvasTextItem CanvasTextItem {
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	CanvasTextItem.Scale = {Scale * 0.75f, Scale * 0.75f};
	CanvasTextItem.EnableShadow(FLinearColor::Black);

	const float RowOffset {12.0f * Scale};
	const float ColumnOffset {120.0f * Scale};

	static const FText ViewRotationText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsViewState, Rotation), false))
	};

	FLinearColor Color {FLinearColor::Red};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = ViewRotationText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("R: %.2f P: %.2f Y: %.2f"),
	                                                      ViewState.Rotation.Roll, ViewState.Rotation.Pitch,
	                                                      ViewState.Rotation.Yaw));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

#if ENABLE_DRAW_DEBUG
	DrawDebugCone(GetWorld(), GetPawnViewLocation(),
	              ViewState.Rotation.Vector(), 100.0f, FMath::DegreesToRadians(15.0f), FMath::DegreesToRadians(15.0f),
	              8, Color.ToFColor(true), false, -1.0f, SDPG_World, 1.0f);
#endif

	VerticalLocation += RowOffset;

	static const FText InputYawAngleText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionState, InputYawAngle), false))
	};

	Color = LocomotionState.bHasInput ? FLinearColor {1.0f, 0.5f, 0.0f} : FLinearColor {0.5f, 0.25f, 0.0f};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = InputYawAngleText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.InputYawAngle));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

#if ENABLE_DRAW_DEBUG
	const FVector FeetLocation {
		LocomotionState.Location - FVector {0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()}
	};

	DrawDebugDirectionalArrow(GetWorld(),
	                          FeetLocation + FVector {0.0f, 0.0f, 3.0f},
	                          FeetLocation + FVector {0.0f, 0.0f, 3.0f} +
	                          UAlsMath::AngleToDirectionXY(LocomotionState.InputYawAngle) * 50.0f,
	                          50.0f, Color.ToFColor(true), false, -1.0f, SDPG_World, 3.0f);
#endif

	VerticalLocation += RowOffset;

	static const FText SpeedText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionState, Speed), false))
	};

	Color = LocomotionState.bHasSpeed ? FLinearColor {0.75f, 0.0f, 1.0f} : FLinearColor {0.375f, 0.0f, 0.5f};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = SpeedText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.Speed));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText VelocityDirectionText {FText::AsCultureInvariant(TEXT("Velocity Direction"))};

	const FVector VelocityDirection {LocomotionState.Velocity.GetSafeNormal()};

	CanvasTextItem.Text = VelocityDirectionText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("X: %.2f Y: %.2f Z: %.2f"),
	                                                      VelocityDirection.X, VelocityDirection.Y,
	                                                      VelocityDirection.Z));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText VelocityYawAngleText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionState, VelocityYawAngle), false))
	};

	CanvasTextItem.Text = VelocityYawAngleText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.VelocityYawAngle));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

#if ENABLE_DRAW_DEBUG
	DrawDebugDirectionalArrow(GetWorld(),
	                          FeetLocation,
	                          FeetLocation +
	                          UAlsMath::AngleToDirectionXY(LocomotionState.VelocityYawAngle) *
	                          FMath::GetMappedRangeValueClamped(FVector2f {0.0f, GetCharacterMovement()->GetMaxSpeed()},
	                                                            {50.0f, 75.0f}, LocomotionState.Speed),
	                          50.0f, Color.ToFColor(true), false, -1.0f, SDPG_World, 3.0f);
#endif

	VerticalLocation += RowOffset;

	static const FText TargetYawAngleText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(
				GET_MEMBER_NAME_STRING_CHECKED(FAlsLocomotionState, SmoothTargetYawAngle), false))
	};

	Color = {0.0f, 0.75f, 1.0f};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = TargetYawAngleText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), LocomotionState.SmoothTargetYawAngle));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

#if ENABLE_DRAW_DEBUG
	DrawDebugDirectionalArrow(GetWorld(),
	                          FeetLocation + FVector {0.0f, 0.0f, 6.0f},
	                          FeetLocation + FVector {0.0f, 0.0f, 6.0f} +
	                          UAlsMath::AngleToDirectionXY(LocomotionState.SmoothTargetYawAngle) * 50.0f,
	                          50.0f, Color.ToFColor(true), false, -1.0f, SDPG_World, 3.0f);
#endif

	VerticalLocation += RowOffset;

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsule(GetWorld(), LocomotionState.Location, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
	                 GetCapsuleComponent()->GetScaledCapsuleRadius(), LocomotionState.RotationQuaternion,
	                 FColor::Green, false, -1.0f, SDPG_World, 1.0f);
#endif
}

void AAlsCharacter::DisplayDebugTraces(const UCanvas* Canvas, const float Scale,
                                       const float HorizontalLocation, float& VerticalLocation) const
{
	VerticalLocation += 4.0f * Scale;

	FCanvasTextItem CanvasTextItem {
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	CanvasTextItem.Scale = {Scale * 0.75f, Scale * 0.75f};
	CanvasTextItem.EnableShadow(FLinearColor::Black);

	const float RowOffset {12.0f * Scale};

	static const FText FootOffsetTraceText {FText::AsCultureInvariant(TEXT("Foot Offset"))};

	CanvasTextItem.SetColor({0.0f, 0.75f, 1.0f});

	CanvasTextItem.Text = FootOffsetTraceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText GroundPredictionTraceText {FText::AsCultureInvariant(TEXT("Ground Prediction"))};

	CanvasTextItem.SetColor({0.75f, 0.0f, 1.0f});

	CanvasTextItem.Text = GroundPredictionTraceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText FootstepEffectsTraceText {FText::AsCultureInvariant(TEXT("Footstep Effects"))};

	CanvasTextItem.SetColor(FLinearColor::Red);

	CanvasTextItem.Text = FootstepEffectsTraceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;
}

void AAlsCharacter::DisplayDebugMantling(const UCanvas* Canvas, const float Scale,
                                         const float HorizontalLocation, float& VerticalLocation) const
{
	VerticalLocation += 4.0f * Scale;

	FCanvasTextItem CanvasTextItem {
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	CanvasTextItem.Scale = {Scale * 0.75f, Scale * 0.75f};
	CanvasTextItem.EnableShadow(FLinearColor::Black);

	const float RowOffset {12.0f * Scale};

	static const FText ForwardTraceText {FText::AsCultureInvariant(TEXT("Forward Trace"))};

	CanvasTextItem.SetColor({0.0f, 0.75f, 1.0f});

	CanvasTextItem.Text = ForwardTraceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText DownwardTraceText {FText::AsCultureInvariant(TEXT("Downward Trace"))};

	CanvasTextItem.SetColor({0.75f, 0.0f, 1.0f});

	CanvasTextItem.Text = DownwardTraceText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText FreeSpaceOverlapText {FText::AsCultureInvariant(TEXT("Free Space Overlap (Only if Failed)"))};

	CanvasTextItem.SetColor(FLinearColor::Red);

	CanvasTextItem.Text = FreeSpaceOverlapText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;
}
