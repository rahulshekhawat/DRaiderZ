// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODBlueprintFunctionLibrary.h"

#include "GameFramework/Actor.h"

float UEODBlueprintFunctionLibrary::CalculateYawDiffBetweenActors(const AActor* Actor1, const AActor* Actor2)
{
	return FMath::FindDeltaAngleDegrees(Actor1->GetActorRotation().Yaw, Actor2->GetActorRotation().Yaw);
}

float UEODBlueprintFunctionLibrary::CalculateYawDiffBetweenRotators(const FRotator& Rotator1, const FRotator& Rotator2)
{
	return FMath::FindDeltaAngleDegrees(Rotator1.Yaw, Rotator2.Yaw);
}

float UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(const FVector& Vector1, const FVector& Vector2)
{
	FVector NormalizedVec1 = Vector1.GetSafeNormal();
	FVector NormalizedVec2 = Vector2.GetSafeNormal();
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedVec1, NormalizedVec2)));
}

float UEODBlueprintFunctionLibrary::FindDeltaAngleDegrees(const float& A1, const float& A2)
{
	return FMath::FindDeltaAngleDegrees(A1, A2);
}

USoundBase* UEODBlueprintFunctionLibrary::GetRandomSound(const TArray<USoundBase*>& Sounds)
{
	USoundBase* Sound = nullptr;

	int32 SoundsNum = Sounds.Num();
	if (SoundsNum == 1)
	{
		Sound = Sounds[0];
	}
	else if (SoundsNum > 1)
	{
		int32 RandSoundIndex = FMath::RandRange(0, SoundsNum - 1);
		Sound = Sounds[RandSoundIndex];
	}

	return Sound;
}
