// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EODLibrary.generated.h"

#define MIN_SCREEN_WIDTH 1024
#define MIN_SCREEN_HEIGHT 768

class UTexture;
class UGameSingleton;
class UGameUserSettings;

UENUM(BlueprintType)
enum class EEODContainerType : uint8
{
	None,
	Inventory,
	SkillTree,
	SkillBar,
};

UENUM(BlueprintType)
enum class EEODItemType : uint8
{
	None,
	ActiveSkill,
	PassiveSkill,
	Armor,
	Weapon,
	Necklace,
	Belt,
	Ring,
	Earring,
	Ingredient,
	Consumable,
	QuestItem,
	Potion,
	Scrap,
};

UENUM(BlueprintType)
enum class EEODTaskStatus : uint8
{
	Inactive,
	Active,
	Finished,
	Aborted
};

USTRUCT(BlueprintType)
struct EOD_API FEODItemInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	FString ItemGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	FString InGameName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	EEODItemType EODItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	int32 StackCount;

	FEODItemInfo()
	{
		ItemID = NAME_None;
		ItemGroup = FString("");
		Icon = nullptr;
		InGameName = FString("");
		Description = FString("");
		EODItemType = EEODItemType::None;
		StackCount = 1;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FInventoryItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory Item")
	FName ItemID;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory Item")
	UTexture* ItemIcon;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Inventory Item")
	FString Description;

	// @todo hover info
};

/**
 * 
 */
UCLASS()
class EOD_API UEODLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "EOD Library")
	static UGameSingleton* GetGameSingleton();
	
	static UGameUserSettings* GetGameUserSettings();
	
};

template<typename ObjType = UObject>
ObjType* EODLoadAsset(TSoftObjectPtr<ObjType> SoftAssetPtr)
{
	ObjType* Obj = nullptr;

	if (SoftAssetPtr.IsNull())
	{
		// pass
	}
	else if (SoftAssetPtr.IsPending())
	{
		Obj = SoftAssetPtr.LoadSynchronous();
	}
	else if (SoftAssetPtr.IsValid())
	{
		Obj = SoftAssetPtr.Get();
	}

	return Obj;
}
