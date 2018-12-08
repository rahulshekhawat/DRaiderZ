// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/WeaponLibrary.h"

#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

class UTexture;
class USkeletalMesh;
class UStatusEffectBase;
class UElementalBase;

/**
 * 
 */
UCLASS()
class EOD_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UWeaponDataAsset(const FObjectInitializer& ObjectInitializer);

	/** The in-game name used for this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FString DisplayName;

	/** Weapon description that will be displayed on hovering over the weapon icon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FString Description;

	/** The skeletal mesh that represents this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	/** Icon that will be used to represent this weapon inside in-game UI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	TSoftObjectPtr<UTexture> Icon;

	/**
	 * Weapon Type determines:
	 * - whether the weapon is singled handed or dual handed.
	 * - the skills that can be used along with this weapon.
	 *
	 * By Default:
	 * - shield is secondary weapon
	 * - Dagger is a hybrid weapon
	 * - everything else is a primary weapon.
	 * - shield, longsword, dagger, and mace are single handed weapons
	 * - greatswords, warhammer, and staff dual handed weapons
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	EWeaponType WeaponType;

	/** Minimum level that a character must be to equip this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	int UnlockLevel;

	/** Maximum damage a weapon can block without consuming more stamina than OnBlock_MinStaminaConsumption */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float Stability;

	/** Maximum damage a weapon can block without consuming more stamina than OnBlock_MinStaminaConsumption */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float DamageThreshold;

	/** Physical damage power of weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	int PhysicalAttack;

	/** Magickal damage power of weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	int MagickalAttack;

	/** Chance of causing a physical critical damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float PhysicalCritRate;

	/** Chance of causing a magickal critical damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float MagickalCritRate;

	/** Bonus physical damage caused on a successful physical critical hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float PhysicalCritBonusDamage;

	/** Bonus magickal damage caused on a successful magickal critical hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float MagickalCritBonusDamage;

	/** Minimum amount of stamina consumed on blocking damage less than DamageThreshold */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	int OnBlock_MinStaminaConsumption;

	/** Percentage of damage reduction on blocking a physical attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float OnBlock_PhysicalDamageReduction;

	/** Percentage of damage reduction on blocking a magickal attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	float OnBlock_MagickalDamageReduction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Stats)
	uint8 MaxEnchantLevel;

	/** Weapon quality scale determines how much the weapon strengthens on enchant */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Stats)
	float WeaponQualityScale;

	/** Determines whether this weapon can be enchanted elementally */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Stats)
	bool bHasElementalEnchantSlot;

	/** Determines whether a special ability gem can be added to weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Stats)
	bool bHasSpecialGemSlot;

	/** Status effects (both buffs and debuffs) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Stats)
	TArray<TSubclassOf<UStatusEffectBase>> StatusEffects;

	/**
	 * Default elemental affinity of this weapon
	 * @note The weapon won't support elemental enchant if the elemental affinity is not none
	 */
	 UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = Stats)
	 TSubclassOf<UElementalBase> ElementalAffinity;

};
