// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/Components/BaseStatsComponent.h"
#include "PrimeStatsComponent.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UPrimeStatsComponent : public UBaseStatsComponent
{
	GENERATED_BODY()
	
public:

	UPrimeStatsComponent();
	
	// Property replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 ManaRegenRate;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 StaminaRegenRate;


};
