// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EODBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UEODBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = Angle)
	static float CalculateYawDiffBetweenActors(const AActor* Actor1, const AActor* Actor2);

	UFUNCTION(BlueprintCallable, Category = Angle)
	static float CalculateYawDiffBetweenRotators(const FRotator& Rotator1, const FRotator& Rotator2);

	/** Returns angle between two vectors in degrees */
	UFUNCTION(BlueprintCallable, Category = Angle)
	static float CalculateAngleBetweenVectors(const FVector& Vector1, const FVector& Vector2);

	UFUNCTION(BlueprintCallable, Category = Angle)
	static float FindDeltaAngleDegrees(float A1, float A2);

};
