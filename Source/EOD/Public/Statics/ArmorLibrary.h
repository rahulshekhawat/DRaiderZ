// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "ArmorLibrary.generated.h"

class UTexture;
class USkeletalMesh;
class UGameplayEffectBase;

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Chest,
	Hands,
	Legs,
	Feet,
	None
};

USTRUCT(BlueprintType)
struct EOD_API FArmorTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = BaseInfo)
	FString DisplayName;
	
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	FString Description;
	
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	TSoftObjectPtr<USkeletalMesh> ArmorMesh;
	
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	TSoftObjectPtr<UTexture> Icon;
	
	UPROPERTY(EditAnywhere, Category = BaseInfo)
	EArmorType ArmorType;

	/** Minimum level that a character must be to equip this Armor */
	UPROPERTY(EditAnywhere, Category = Stats)
	int32 UnlockLevel;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int32 PhysicalResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	int32 MagickalResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float PhysicalCritRate;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float MagickalCritRate;

	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	int32 MaxEnchantLevel;

	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	TMap<FName, float> AdditionalStatsBonus;
	
	/** Armor quality scale determines how much the armor strengthens on enchant */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	float ArmorQualityScale;

	/** A map of trigger condition to it's gameplay effect that it can trigger */
	UPROPERTY(EditAnywhere, Category = AdditionalInfo)
	TMap<FName, UGameplayEffectBase*> StatusEffects;

	FArmorTableRow() :
		ArmorMesh(NULL),
		Icon(NULL),
		ArmorType(EArmorType::None),
		UnlockLevel(1),
		PhysicalResistance(0),
		MagickalResistance(0),
		PhysicalCritRate(0.f),
		MagickalCritRate(0.f),
		MaxEnchantLevel(5),
		ArmorQualityScale(1.f)
	{
	}

};

/**
 * ArmorLibrary contains static helper functions for handling in-game armor and armor types.
 * @note Do not derive from this class.
 * @warning This may get removed in future iterations of code since any static functions required can be declared inside CharacterLibrary
 */
UCLASS()
class EOD_API UArmorLibrary : public UObject
{
	GENERATED_BODY()

public:

	/** Returns FWeaponData corresponding to WeaponID from the weapons data table */
	static FArmorTableRow* GetArmorData(FName ArmorID);

	
	
	
};
