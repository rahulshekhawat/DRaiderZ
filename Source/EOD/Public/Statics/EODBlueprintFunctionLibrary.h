// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EODBlueprintFunctionLibrary.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class EOD_API UEODBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** Returns the yaw difference between two actors in degrees */
	UFUNCTION(BlueprintCallable, Category = "EOD Library|Angle")
	static float CalculateYawDiffBetweenActors(const AActor* Actor1, const AActor* Actor2);

	/** Returns the yaw difference between two rotators in degrees */
	UFUNCTION(BlueprintCallable, Category = "EOD Library|Angle")
	static float CalculateYawDiffBetweenRotators(const FRotator& Rotator1, const FRotator& Rotator2);

	/** Returns angle between two vectors in degrees */
	UFUNCTION(BlueprintCallable, Category = "EOD Library|Angle")
	static float CalculateAngleBetweenVectors(const FVector& Vector1, const FVector& Vector2);

	UFUNCTION(BlueprintCallable, Category = "EOD Library|Angle")
	static float FindDeltaAngleDegrees(const float& A1, const float& A2);

	/** Returns a random sound out of a collection of sounds */
	UFUNCTION(BlueprintCallable, Category = "EOD Library|Sound")
	static USoundBase* GetRandomSound(const TArray<USoundBase*>& Sounds);

};
