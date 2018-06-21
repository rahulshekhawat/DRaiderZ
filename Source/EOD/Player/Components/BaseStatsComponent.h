// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API UBaseStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UBaseStatsComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Property replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:

	/** Character Level */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 Level;
	
	// UPROPERTY(Replicated, EditDefaultsOnly, Category = StatusIndicator)
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 Darkness;

	/** Maximum health of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseHealth;

	/** Current maximum health of character - with or without any status effects */
	UPROPERTY(Replicated)
	int32 MaxHealth;

	/** Current health of character */
	UPROPERTY(Replicated)
	int32 CurrentHealth;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	int32 PhysicalAttack;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats)
	int32 PhysicalResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	int32 MagickAttack;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats)
	int32 MagickResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 HealthRegenRate;	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalFireDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalIceDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalPoisonDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalLightningDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalHolyDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalDarkDamage;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalFireResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalIceResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalPoisonResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalLightningResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalHolyResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalDarkResistance;
		
	
};
