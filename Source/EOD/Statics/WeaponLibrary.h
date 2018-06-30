// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "WeaponLibrary.generated.h"

UENUM(BlueprintType)
enum class EWeaponAnimationType : uint8
{
	NoWeapon,
	SheathedWeapon,
	ShieldAndSword,
	ShieldAndMace,
	GreatSword,
	WarHammer,
	Staff,
	Daggers
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	GreatSword,
	WarHammer,
	LongSword,
	Mace,
	Dagger,
	Staff,
	Shield
};

USTRUCT(BlueprintType)
struct EOD_API FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	/** The in-game name used for this weapon */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	FText DisplayName;
	
	/** Weapon description that will be displayed on hovering over the weapon icon */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	FText Description;
	
	/** The skeletal mesh that represents this weapon */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	TSoftObjectPtr<class USkeletalMesh> WeaponMesh;
	
	/** Icon that will be used to represent this weapon inside in-game UI */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	TSoftObjectPtr<class UTexture> Icon;
	
	/**
	 * Weapon Type determines:
	 * - whether the weapon is singled handed or dual handed.
	 * - the skills that can be used along with this weapon.
	 * 
	 * By Default:
	 * - shield is secondary weapon and every weapon except shield is primary weapon.
	 * - shield, longsword, and mace are single handed weapons
	 * - greatswords, warhammer, staff, and daggers are dual handed weapons
	 *
	 * Note: Daggers are dual handed because player is not allowed tp equip just a single dagger.
	 */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	EWeaponType WeaponType;

	/** Minimum level required to equip this weapon */
	UPROPERTY(EditAnywhere, Category = Stats)
	int Level;
	
	/** Maximum damage a weapon can block without consuming more stamina than OnBlock_MinStaminaConsumption */
	UPROPERTY(EditAnywhere, Category = Stats)
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
	float OnBlock_PhysicalDamageReduction = 50.f;
	
	/** Percentage of damage reduction on blocking a magickal attack */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float OnBlock_MagickalDamageReduction = 50.f;

	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	uint8 MaxEnchantLevel = 5;
	
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	float WeaponQualityScale;
	
	/** Determines whether this weapon can be enchanted elementally */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	bool bSupportsElementalEnchant;

	/** Determines whether a special ability gem can be added to weapon */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	bool bCanAddSpecialAbilityGem;
	
	/** Status effects (both buffs and debuffs) */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	TArray<TSubclassOf<class UStatusEffect>> StatusEffects;
	
	/**
	 * Default elemental affinity of this weapon
	 * @note The weapon won't support elemental enchant if the elemental affinity is not none
	 */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	TSubclassOf<class UBaseElemental> ElementalAffinity;

};


/**
 * 
 */
UCLASS()
class EOD_API UWeaponLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	static bool IsWeaponDualHanded(EWeaponType WeaponType);
	
	static bool IsWeaponSingleHanded(EWeaponType WeaponType);

	static bool IsPrimaryWeapon(EWeaponType WeaponType);

	static bool IsSecondaryWeapon(EWeaponType WeaponType);
	
	static FWeaponData* GetWeaponData(FName WeaponID);
	
	
	
};
