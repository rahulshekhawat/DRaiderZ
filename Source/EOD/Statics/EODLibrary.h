// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EODLibrary.generated.h"

class UTexture;

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

USTRUCT(BlueprintType)
struct EOD_API FEODItemInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	FName ItemID;

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

};

/**
 * 
 */
UCLASS()
class EOD_API UEODLibrary : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
