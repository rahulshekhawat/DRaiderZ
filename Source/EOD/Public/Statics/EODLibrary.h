// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "EODLibrary.generated.h"


class UTexture;
class USoundBase;
class UGameSingleton;
class UGameUserSettings;
class UContainerWidget;

UENUM(BlueprintType)
enum class ERarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Legendary
};

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	// Items like scrap, recipe, etc. that cannot be used or equipped
	None,
	// A item that is consumed when used
	Consumable,
	// A item that can be used again and again (preferably with some cooldown limitations)
	Reusable,
	// An equippable item gets removed from inventory when equipped
	Equippable
};

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

UENUM(BlueprintType)
enum class EWidgetCreationStatus : uint8
{
	Unknown,
	WaitingCreation,
	CannotBeCreated,
	SuccessfullyCreated
};

UENUM(BlueprintType)
enum class EWidgetInitializationStatus : uint8
{
	Unknown,
	WaitingInitialization,
	CannotBeInitialized,
	Initialized
};

USTRUCT(BlueprintType)
struct EOD_API FInGameInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-game Information")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-game Information")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-game Information")
	class UTexture* Icon;

	FInGameInformation() :
		Name(TEXT("")),
		Description(TEXT("")),
		Icon(nullptr)
	{
	}
};

/*
USTRUCT(BlueprintType)
struct EOD_API FUIInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Information")
	FString Title;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Information")
	FString SubTitle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Information")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Information")
	class UTexture* Icon;

};
*/

USTRUCT(BlueprintType)
struct EOD_API FGameplayTagMod
{
	GENERATED_USTRUCT_BODY()

	FGameplayTagContainer Tags;

	TSoftObjectPtr<UObject> SourceObj;

	FGameplayTagMod() : SourceObj(NULL)
	{
	}

	FGameplayTagMod(FGameplayTagContainer TagContainer, UObject* SourceObject) :
		Tags(TagContainer),
		SourceObj(SourceObject)
	{
	}

	bool operator==(FGameplayTagMod const& Other) const;
	bool operator!=(FGameplayTagMod const& Other) const;

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
};

USTRUCT(BlueprintType)
struct EOD_API FInventorySlot
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UObject> ItemClass;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;				// Not uint32 only because uint32 is not supported in blueprints

	UPROPERTY(BlueprintReadOnly)
	int32 ItemStackCount;			// Not uint32 only because uint32 is not supported in blueprints

	UPROPERTY(BlueprintReadOnly)
	class UInventoryItemBase* ItemInSlot;

	/** Container widget associated with this inventory slot */
	UPROPERTY(BlueprintReadOnly)
	class UInventoryContainerWidget* SlotWidget;

	bool IsEmpty() const
	{
		return ItemInSlot == nullptr;
	}

	bool IsValid() const
	{
		//~ @todo check if the SlotIndex is valid
		return true;
	}

	FInventorySlot() :
		SlotIndex(-1),
		ItemStackCount(-1),
		ItemInSlot(nullptr),
		SlotWidget(nullptr)
	{
	}
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
