// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODBlueprintFunctionLibrary.h"

#include "GameFramework/Actor.h"

float UEODBlueprintFunctionLibrary::CalculateYawDiffBetweenActors(const AActor * Actor1, const AActor * Actor2)
{
	return FMath::FindDeltaAngleDegrees(Actor1->GetActorRotation().Yaw, Actor2->GetActorRotation().Yaw);
}

float UEODBlueprintFunctionLibrary::CalculateYawDiffBetweenRotators(const FRotator & Rotator1, const FRotator & Rotator2)
{
	return FMath::FindDeltaAngleDegrees(Rotator1.Yaw, Rotator2.Yaw);
}

float UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(const FVector & Vector1, const FVector & Vector2)
{
	FVector NormalizedVec1 = Vector1.GetSafeNormal();
	FVector NormalizedVec2 = Vector2.GetSafeNormal();
	float Angle = FMath::Acos(FVector::DotProduct(NormalizedVec1, NormalizedVec2));
	return Angle;
}
