#include "Utility/AlsMath.h"

template <class ValueType, class StateType>
ValueType UAlsMath::SpringDamp(const ValueType& Current, const ValueType& Target, StateType& SpringState, const float DeltaTime,
                               const float Frequency, const float DampingRatio, const float TargetVelocityAmount)
{
	if (DeltaTime <= SMALL_NUMBER)
	{
		return Current;
	}

	if (!SpringState.bStateValid)
	{
		SpringState.Velocity = ValueType{0.0f};
		SpringState.PreviousTarget = Target;
		SpringState.bStateValid = true;

		return Target;
	}

	ValueType Result{Current};
	FMath::SpringDamper(Result, SpringState.Velocity, Target,
	                    (Target - SpringState.PreviousTarget) * (Clamp01(TargetVelocityAmount) / DeltaTime),
	                    DeltaTime, Frequency, DampingRatio);

	SpringState.PreviousTarget = Target;

	return Result;
}

float UAlsMath::SpringDampFloat(const float Current, const float Target, FAlsSpringFloatState& SpringState,
                                const float DeltaTime, const float Frequency, const float DampingRatio, const float TargetVelocityAmount)
{
	return SpringDamp(Current, Target, SpringState, DeltaTime, Frequency, DampingRatio, TargetVelocityAmount);
}

FVector UAlsMath::SpringDampVector(const FVector& Current, const FVector& Target, FAlsSpringVectorState& SpringState,
                                   const float DeltaTime, const float Frequency, const float DampingRatio, const float TargetVelocityAmount)
{
	return SpringDamp(Current, Target, SpringState, DeltaTime, Frequency, DampingRatio, TargetVelocityAmount);
}

FVector UAlsMath::SlerpSkipNormalization(const FVector& From, const FVector& To, const float Alpha)
{
	// http://allenchou.net/2018/05/game-math-deriving-the-slerp-formula/

	const auto Dot{From | To};

	if (Dot > 0.9995 || Dot < -0.9995)
	{
		return FMath::Lerp(From, To, Alpha).GetSafeNormal();
	}

	const double Theta = FMath::Acos(Dot) * Alpha;

	double Sin, Cos;
	FMath::SinCos(&Sin, &Cos, Theta);

	const auto FromPerpendicular{(To - From * Dot).GetSafeNormal()};

	return From * Cos + FromPerpendicular * Sin;
}

EAlsMovementDirection UAlsMath::CalculateMovementDirection(const float Angle, const float ForwardHalfAngle, const float AngleThreshold)
{
	if (Angle >= -ForwardHalfAngle - AngleThreshold && Angle <= ForwardHalfAngle + AngleThreshold)
	{
		return EAlsMovementDirection::Forward;
	}

	if (Angle >= ForwardHalfAngle - AngleThreshold && Angle <= 180.0f - ForwardHalfAngle + AngleThreshold)
	{
		return EAlsMovementDirection::Right;
	}

	if (Angle <= -(ForwardHalfAngle - AngleThreshold) && Angle >= -(180.0f - ForwardHalfAngle + AngleThreshold))
	{
		return EAlsMovementDirection::Left;
	}

	return EAlsMovementDirection::Backward;
}
