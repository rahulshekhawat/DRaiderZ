// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API UWeaponStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// @todo remove Level from weapon stats if it's not needed
	/**
	 * Weapon level determines what level player needs to be before equipping the weapon.
	 * @note This stat might not be necessary.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int Level;
	
	/** Determines how efficiently a weapon can block incoming attacks */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float Stability;
	
	/** Maximum damage a weapon can block without consuming more stamina than OnBlock_MinStaminaConsumption */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float DamageThreshold;

	/** Physical damage power of weapon */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int PhysicalAttack;
	
	/** Magickal damage power of weapon */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int MagickalAttack;
	
	/** Chance of causing a physical critical damage */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float PhysicalCritRate;
	
	/** Chance of causing a magickal critical damage */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float MagickalCritRate;
	
	/** Bonus physical damage caused on a successful physical critical hit */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float PhysicalCritBonusDamage;
	
	/** Bonus magickal damage caused on a successful magickal critical hit */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float MagickalCritBonusDamage;
	
	/** Minimum amount of stamina consumed on blocking damage less than DamageThreshold */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int OnBlock_MinStaminaConsumption;
	
	/** Percentage of damage reduction on blocking a physical attack */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int OnBlock_PhysicalDamageReduction;
	
	/** Percentage of damage reduction on blocking a magickal attack */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int OnBlock_MagickalDamageReduction;
		
	
};
