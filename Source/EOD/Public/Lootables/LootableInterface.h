// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "LootableInterface.generated.h"

// class UUserWidget;
class AEODCharacterBase;

USTRUCT(BlueprintType)
struct EOD_API FStoredLootInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UObject> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCount;

	/** Chance for this item to drop between 0 and 100% */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DropChance;

};

USTRUCT(BlueprintType)
struct EOD_API FGeneratedLootInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UObject> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemCount;

};

/** An interface that must be implemented for all in-game interactive actors */
UINTERFACE(BlueprintType)
class EOD_API ULootableInterface : public UInterface
{
	GENERATED_BODY()

public:

    ULootableInterface(const FObjectInitializer& ObjectInitializer);

};

/** An interface that must be implemented for all in-game interactive actors */
class EOD_API ILootableInterface
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  Lootable Interface
	// --------------------------------------

	virtual void GenerateLootInfoArray();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Lootable)
	TArray<FStoredLootInfo> GetStoredLootInfo() const;
	virtual TArray<FStoredLootInfo> GetStoredLootInfo_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Lootable)
	TArray<FGeneratedLootInfo> GetGeneratedLootInfo() const;
	virtual TArray<FGeneratedLootInfo> GetGeneratedLootInfo_Implementation() const;

	/**
	 * @param LootItemClass	ItemClass that we want to acquire loot of.
	 * @param Looter		Character that initiated the loot.
	 * @return				Number of acquired loot items
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Lootable)
	int32 AcquireLootItem(TSubclassOf<UObject> LootItemClass, AEODCharacterBase* Looter);
	virtual int32 AcquireLootItem_Implementation(TSubclassOf<UObject> LootItemClass, AEODCharacterBase* Looter);

};
