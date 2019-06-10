// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// #include "StatusEffectBase.h"

#include "Engine/Texture.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/NoExportTypes.h"
#include "WeaponLibrary.generated.h"

class UWeaponDataAsset;
class UGameplayEffectBase;

/** This enum describes all the types of weapons available in-game */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	GreatSword,
	WarHammer,
	LongSword,
	Mace,
	Dagger,
	Staff,
	Shield,
	None
};

//~ @todo FWeaponData vs WeaponStatsComponent, decide which one to keep

USTRUCT(BlueprintType)
struct EOD_API FWeaponTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** The in-game name used for this weapon */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	FString DisplayName;
	
	/** Weapon description that will be displayed on hovering over the weapon icon */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	FString Description;
	
	/** The skeletal mesh that represents this weapon */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
	
	/** Icon that will be used to represent this weapon inside in-game UI */
	UPROPERTY(EditAnywhere, Category = BaseInfo)
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
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	EWeaponType WeaponType;

	/** Minimum level that a character must be to equip this weapon */
	UPROPERTY(EditAnywhere, Category = Stats)
	int32 UnlockLevel;
	
	/** Maximum damage a weapon can block without consuming more stamina than OnBlock_MinStaminaConsumption */
	UPROPERTY(EditAnywhere, Category = Stats)
	float Stability;
	
	/** Maximum damage a weapon can block without consuming more stamina than OnBlock_MinStaminaConsumption */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float DamageThreshold;

	/** Physical damage power of weapon */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int32 PhysicalAttack;
	
	/** Magickal damage power of weapon */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int32 MagickalAttack;
	
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
	int32 OnBlock_MinStaminaConsumption;
	
	/** Percentage of damage reduction on blocking a physical attack */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float OnBlock_PhysicalDamageReduction = 50.f;
	
	/** Percentage of damage reduction on blocking a magickal attack */
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float OnBlock_MagickalDamageReduction = 50.f;

	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	int32 MaxEnchantLevel;
	
	/** Weapon quality scale determines how much the weapon strengthens on enchant */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	float WeaponQualityScale;
	
	/** Determines whether this weapon can be enchanted elementally */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	bool bSupportsElementalEnchant;

	/** Determines whether a special ability gem can be added to weapon */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	bool bHasSpecialGemSlot;
	
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	TMap<FName, float> AdditionalStatsBonus;
	
	/** A map of trigger condition to it's gameplay effect that it can trigger */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	TMap<FName, UGameplayEffectBase*> StatusEffects;

	FWeaponTableRow() :
		WeaponMesh(NULL),
		Icon(NULL),
		WeaponType(EWeaponType::None),
		UnlockLevel(1),
		Stability(0.f),
		DamageThreshold(0.f),
		PhysicalAttack(0),
		MagickalAttack(0),
		PhysicalCritRate(0.f),
		MagickalCritRate(0.f),
		PhysicalCritBonusDamage(0.f),
		MagickalCritBonusDamage(0.f),
		OnBlock_MinStaminaConsumption(0),
		OnBlock_PhysicalDamageReduction(50.f),
		OnBlock_MagickalDamageReduction(50.f),
		MaxEnchantLevel(5),
		WeaponQualityScale(1.f),
		bSupportsElementalEnchant(false),
		bHasSpecialGemSlot(false)
	{
	}
};


/**
 * WeaponLibrary contains static helper functions for handling in-game weapon and weapon types.
 * @note Do not derive from this class
 */
UCLASS()
class EOD_API UWeaponLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	/** Returns true if the weapon requires both hands to wield it */
	static bool IsWeaponDualHanded(EWeaponType WeaponType);
	
	/** Returns true if the weapon can be wielded with single hand */
	static bool IsWeaponSingleHanded(EWeaponType WeaponType);

	/**
	 * Returns true if weapon is a primary weapon type, i.e., weapon can be equipped in primary (right) hand
	 * @note Dual handed weapons count as primary weapons
	 * @see APrimaryWeapon
	 */
	static bool IsPrimaryWeapon(EWeaponType WeaponType);

	/**
	 * Returns true if weapon is a secondary weapon type, i.e., weapon can be equipped in secondary (left) hand
	 * @see ASecondaryWeapon
	 */
	static bool IsSecondaryWeapon(EWeaponType WeaponType);

	/** Returns true if weapon is a hybrid weapon type, i.e., weapon can be equipped in either primary or secondary hand */
	static bool IsHybridWeapon(EWeaponType WeaponType);
	
	/** Returns FWeaponData corresponding to WeaponID from the weapons data table */
	static FWeaponTableRow* GetWeaponData(FName WeaponID);
	
};
