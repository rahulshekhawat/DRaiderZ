// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "LootableInterface.generated.h"

class AEODCharacterBase;

USTRUCT(BlueprintType)
struct EOD_API FStoredLootInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/** A subclass of UObject that implements the IInventoryInterface */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Loot)
	TSubclassOf<class UObject> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Loot)
	int32 MaxCount;

	/** Chance for this item to drop between 0 and 1 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Loot)
	float DropChance;

};

USTRUCT(BlueprintType)
struct EOD_API FGeneratedLootInfo
{
	GENERATED_USTRUCT_BODY()

	/** Generated Loot ID */
	UPROPERTY(Transient, BlueprintReadOnly, Category = Loot)
	int32 GLID;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Loot)
	TSubclassOf<class UObject> ItemClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Loot)
	int32 ItemCount;
	
	bool operator==(const FGeneratedLootInfo& Other)
	{
		return this->GLID == Other.GLID;
	}
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
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Lootable)
	void AcquireLoot(const FGeneratedLootInfo& LootInfo, class AEODPlayerController* EODPC);
	virtual void AcquireLoot_Implementation(const FGeneratedLootInfo& LootInfo, class AEODPlayerController* EODPC);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Lootable)
	void OnAllLootPicked(class AEODPlayerController* EODPC);
	virtual void OnAllLootPicked_Implementation(class AEODPlayerController* EODPC);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Lootable)
	void OnLootCancelled(class AEODPlayerController* EODPC);
	virtual void OnLootCancelled_Implementation(class AEODPlayerController* EODPC);

};
