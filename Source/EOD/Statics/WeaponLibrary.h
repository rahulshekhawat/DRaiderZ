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

USTRUCT(BlueprintType)
struct EOD_API FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	/** The in-game name used for this weapon */
	UPROPERTY(EditAnywhere)
	FString DisplayName;
	
	/** Weapon description that will be displayed on hovering over the weapon icon */
	UPROPERTY(EditAnywhere)
	FString Description;
	
	/** The Static Model that represents this weapon */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UStaticMesh> StaticModel;
	
	/** Icon that will be used to represent this weapon inside in-game UI */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class UTexture> Icon;

	UPROPERTY(EditAnywhere)
	float OnBlockPhysicalDamageReduction = 50.f;
	
	UPROPERTY(EditAnywhere)
	float OnBlockMagickalDamageReduction = 50.f;
	
	UPROPERTY(EditAnywhere)
	uint8 MaxEnchantLevel = 5;
	
	UPROPERTY(EditAnywhere)
	float WeaponQualityScale;
	
	/** Determines whether this weapon can be enchanted elementally */
	UPROPERTY(EditAnywhere)
	bool bSupportsElementalEnchant;

	UPROPERTY(EditAnywhere)
	bool bCanAddSpecialAbilityGem;
	
	UPROPERTY(EditAnywhere)
	FName StatusEffectID;





};


/**
 * 
 */
UCLASS()
class EOD_API UWeaponLibrary : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
