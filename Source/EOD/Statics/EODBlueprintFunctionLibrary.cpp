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
