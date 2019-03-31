// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponLibrary.h"

#include "Engine/Texture.h"
#include "Engine/DataAsset.h"
#include "Engine/SkeletalMesh.h"
#include "WeaponDataAsset.generated.h"

class UElementalBase;
class UStatusEffectBase;

/**
 * 
 */
UCLASS()
class EOD_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UWeaponDataAsset(const FObjectInitializer& ObjectInitializer);

	inline USkeletalMesh* GetWeaponMesh();

	inline UTexture* GetIcon();

protected:
	/** The skeletal mesh that represents this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	USkeletalMesh* WeaponMesh;

	/** A soft pointer to skeletal mesh that represents this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	TSoftObjectPtr<USkeletalMesh> WeaponMeshSoftPtr;

	/** Weapon's UI icon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	UTexture* Icon;
	
	/** A soft pointer to weapon's UI icon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	TSoftObjectPtr<UTexture> IconSoftPtr;

public:
	/** The in-game name used for this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FString DisplayName;

	/** Weapon description that will be displayed on hovering over the weapon icon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FString Description;

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

USTRUCT(BlueprintType)
struct EOD_API FWeaponDataAssetTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponDAta)
	TSoftObjectPtr<UWeaponDataAsset> WeaponDataAsset;

};

inline USkeletalMesh* UWeaponDataAsset::GetWeaponMesh()
{
	if (IsValid(WeaponMesh))
	{
		return WeaponMesh;
	}
	else
	{
		if (WeaponMeshSoftPtr.IsNull())
		{
			return nullptr;
		}
		else if (WeaponMeshSoftPtr.IsPending())
		{
			return WeaponMeshSoftPtr.LoadSynchronous();
		}
		else if (WeaponMeshSoftPtr.IsValid())
		{
			return WeaponMeshSoftPtr.Get();
		}
	}

	return nullptr;
}

inline UTexture* UWeaponDataAsset::GetIcon()
{
	if (IsValid(Icon))
	{
		return Icon;
	}
	else
	{
		if (IconSoftPtr.IsNull())
		{
			return nullptr;
		}
		else if (IconSoftPtr.IsPending())
		{
			return IconSoftPtr.LoadSynchronous();
		}
		else if (IconSoftPtr.IsValid())
		{
			return IconSoftPtr.Get();
		}
	}

	return nullptr;
}
