// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EODLibrary.generated.h"

#define MIN_SCREEN_WIDTH 1024
#define MIN_SCREEN_HEIGHT 768

class UTexture;
class USoundBase;
class UGameSingleton;
class UGameUserSettings;
class UContainerWidget;

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
	
	UPROPERTY(BlueprintReadOnly, Category = "In-Game Info")
	FString Name;

	UPROPERTY(BlueprintReadOnly, Category = "In-Game Info")
	FString Description;
	
	UPROPERTY(BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(BlueprintReadOnly)
	FName ItemType;
	
	UPROPERTY(BlueprintReadOnly)
	FName ItemCategory;

	UPROPERTY(BlueprintReadOnly)
	UTexture* ItemIcon;

	/** Unique items cannot be stacked */
	UPROPERTY(BlueprintReadOnly)
	bool bIsUnique;

	FInventoryItem() :
		Name(TEXT("")),
		Description(TEXT("")),
		ItemID(NAME_None),
		ItemType(NAME_None),
		ItemCategory(NAME_None),
		ItemIcon(nullptr),
		bIsUnique(false)
	{
	}

	FInventoryItem(const FInventoryItem& SourceItem) :
		Name(SourceItem.Name),
		Description(SourceItem.Description),
		ItemID(SourceItem.ItemID),
		ItemType(SourceItem.ItemType),
		ItemCategory(SourceItem.ItemCategory),
		ItemIcon(SourceItem.ItemIcon),
		bIsUnique(SourceItem.bIsUnique)
	{
	}

	void operator=(const FInventoryItem& OtherItem)
	{
		this->Name = OtherItem.Name;
		this->Description = OtherItem.Description;
		this->ItemID = OtherItem.ItemID;
		this->ItemType = OtherItem.ItemType;
		this->ItemCategory = OtherItem.ItemCategory;
		this->ItemIcon = OtherItem.ItemIcon;
		this->bIsUnique = OtherItem.bIsUnique;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FInventorySlot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;				// Not uint32 only because uint32 is not supported in blueprints

	UPROPERTY(BlueprintReadOnly)
	int32 ItemStackCount;			// Not uint32 only because uint32 is not supported in blueprints

	UPROPERTY(BlueprintReadOnly)
	FInventoryItem ItemInSlot;

	/** Container widget associated with this inventory slot */
	UPROPERTY(BlueprintReadOnly)
	UContainerWidget* SlotWidget;

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

	UFUNCTION(BlueprintCallable, Category = "EOD Library")
	static USoundBase* GetRandomSound(const TArray<USoundBase*> Sounds);

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
