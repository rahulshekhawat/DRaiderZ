// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PhysicalDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UPhysicalDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UPhysicalDamageCalculation();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
