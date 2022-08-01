#include "AlsCameraComponent.h"

#include "DisplayDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Engine/Canvas.h"
#include "Utility/AlsCameraConstants.h"
#include "Utility/AlsUtility.h"

void UAlsCameraComponent::DisplayDebug(const UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay,
                                       float& VerticalLocation) const
{
	const float Scale = FMath::Min(Canvas->SizeX / (1280.0f * Canvas->GetDPIScale()),
	                               Canvas->SizeY / (720.0f * Canvas->GetDPIScale()));

	const float RowOffset {12.0f * Scale};
	const float ColumnOffset {200.0f * Scale};

	float MaxVerticalLocation {VerticalLocation};
	float HorizontalLocation {5.0f * Scale};

	if (!DebugDisplay.IsDisplayOn(UAlsCameraConstants::CameraCurvesDisplayName()) &&
		!DebugDisplay.IsDisplayOn(UAlsCameraConstants::CameraShapesDisplayName()) &&
		!DebugDisplay.IsDisplayOn(UAlsCameraConstants::CameraTracesDisplayName()))
	{
		VerticalLocation = MaxVerticalLocation;
		return;
	}

	const float InitialVerticalLocation {VerticalLocation};

	static const auto CameraCurvesHeaderText{FText::AsCultureInvariant(TEXT("Als.CameraCurves (Shift + 6)"))};

	if (DebugDisplay.IsDisplayOn(UAlsCameraConstants::CameraCurvesDisplayName()))
	{
		DisplayDebugHeader(Canvas, CameraCurvesHeaderText, FLinearColor::Green, Scale, HorizontalLocation,
		                   VerticalLocation);
		DisplayDebugCurves(Canvas, Scale, HorizontalLocation, VerticalLocation);

		MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation + RowOffset);
		VerticalLocation = InitialVerticalLocation;
		HorizontalLocation += ColumnOffset;
	}
	else
	{
		DisplayDebugHeader(Canvas, CameraCurvesHeaderText, {0.0f, 0.333333f, 0.0f}, Scale, HorizontalLocation,
		                   VerticalLocation);

		VerticalLocation += RowOffset;
	}

	MaxVerticalLocation = FMath::Max(MaxVerticalLocation, VerticalLocation);

	static const auto ShapesHeaderText{FText::AsCultureInvariant(TEXT("Als.CameraShapes (Shift + 7)"))};

	if (DebugDisplay.IsDisplayOn(UAlsCameraConstants::CameraShapesDisplayName()))
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

	static const auto TracesHeaderText{FText::AsCultureInvariant(TEXT("Als.CameraTraces (Shift + 8)"))};

	if (DebugDisplay.IsDisplayOn(UAlsCameraConstants::CameraTracesDisplayName()))
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

	VerticalLocation = MaxVerticalLocation;
}

void UAlsCameraComponent::DisplayDebugHeader(const UCanvas* Canvas, const FText& HeaderText,
                                             const FLinearColor& HeaderColor,
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

void UAlsCameraComponent::DisplayDebugCurves(const UCanvas* Canvas, const float Scale,
                                             const float HorizontalLocation, float& VerticalLocation) const
{
	VerticalLocation += 4.0f * Scale;

	FCanvasTextItem Text {
		FVector2D::ZeroVector,
		FText::GetEmpty(),
		GEngine->GetMediumFont(),
		FLinearColor::White
	};

	Text.Scale = {Scale * 0.75f, Scale * 0.75f};
	Text.EnableShadow(FLinearColor::Black);

	const float RowOffset {12.0f * Scale};
	const float ColumnOffset {145.0f * Scale};

	static TArray<FName> CurveNames;
	check(CurveNames.IsEmpty())

	GetAnimInstance()->GetAllCurveNames(CurveNames);

	CurveNames.Sort([](const FName& A, const FName& B) { return A.LexicalLess(B); });

	for (const FName& CurveName : CurveNames)
	{
		const float CurveValue {GetAnimInstance()->GetCurveValue(CurveName)};

		Text.SetColor(FMath::Lerp(FLinearColor::Gray, FLinearColor::White, UAlsMath::Clamp01(FMath::Abs(CurveValue))));

		Text.Text = FText::AsCultureInvariant(FName::NameToDisplayString(CurveName.ToString(), false));
		Text.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

		Text.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), CurveValue));
		Text.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

		VerticalLocation += RowOffset;
	}

	CurveNames.Reset();
}

void UAlsCameraComponent::DisplayDebugShapes(const UCanvas* Canvas, const float Scale,
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

	static const FText PivotTargetLocationText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, PivotTargetLocation), false))
	};

	FLinearColor Color {FLinearColor::Green};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = PivotTargetLocationText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("X: %.2f Y: %.2f Z: %.2f"),
	                                                                PivotTargetLocation.X, PivotTargetLocation.Y,
	                                                                PivotTargetLocation.Z));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText PivotLagLocationText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, PivotLagLocation), false))
	};

	Color = {1.0f, 0.5f, 0.0f};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = PivotLagLocationText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("X: %.2f Y: %.2f Z: %.2f"),
	                                                                PivotLagLocation.X, PivotLagLocation.Y,
	                                                                PivotLagLocation.Z));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText PivotLocationText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, PivotLocation), false))
	};

	Color = {0.0f, 0.75f, 1.0f};
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = PivotLocationText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("X: %.2f Y: %.2f Z: %.2f"),
	                                                                PivotLocation.X, PivotLocation.Y, PivotLocation.Z));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText CameraFovText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, CameraFov), false))
	};

	Color = FLinearColor::White;
	CanvasTextItem.SetColor(Color);

	CanvasTextItem.Text = CameraFovText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(FString::Printf(TEXT("%.2f"), CameraFov));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText RightShoulderText {
		FText::AsCultureInvariant(
			FName::NameToDisplayString(GET_MEMBER_NAME_STRING_CHECKED(ThisClass, bRightShoulder), true))
	};

	CanvasTextItem.Text = RightShoulderText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	CanvasTextItem.Text = FText::AsCultureInvariant(
		FName::NameToDisplayString(UAlsUtility::BoolToString(bRightShoulder), false));
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation + ColumnOffset, VerticalLocation});

	VerticalLocation += RowOffset;
}

void UAlsCameraComponent::DisplayDebugTraces(const UCanvas* Canvas, const float Scale,
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

	static const FText BlockingGeometryAdjustmentText {FText::AsCultureInvariant(TEXT("Blocking Geometry Adjustment"))};

	CanvasTextItem.SetColor({0.0f, 0.75f, 1.0f});

	CanvasTextItem.Text = BlockingGeometryAdjustmentText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText CameraTraceNoHitText {FText::AsCultureInvariant(TEXT("Camera Trace (No Hit)"))};

	CanvasTextItem.SetColor(FLinearColor::Green);

	CanvasTextItem.Text = CameraTraceNoHitText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;

	static const FText CameraTraceBlockingHitText {FText::AsCultureInvariant(TEXT("Camera Trace (Blocking Hit)"))};

	CanvasTextItem.SetColor(FLinearColor::Red);

	CanvasTextItem.Text = CameraTraceBlockingHitText;
	CanvasTextItem.Draw(Canvas->Canvas, {HorizontalLocation, VerticalLocation});

	VerticalLocation += RowOffset;
}
