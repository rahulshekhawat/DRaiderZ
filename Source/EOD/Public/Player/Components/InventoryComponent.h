// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"
#include "LootableInterface.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// class UTexture;
// class APlayerCharacter;
class UInventoryItemBase;
class UDataTable;

/** Delegate for when a new inventory item is added */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemChangedMCDelegate, const FInventoryItem&, InventoryItem);


/**
 * InventoryComponent is used to implement and manage player inventory.
 * Should be attached to player controller, not pawn
 */
UCLASS()
class EOD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Sets default values for this component's properties */
	UInventoryComponent(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts */
	virtual void BeginPlay() override;	

	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// --------------------------------------
	//  Inventory
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryItem>& GetInventoryItems();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventoryItemChangedMCDelegate OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventoryItemChangedMCDelegate OnInventoryItemRemoved;

	/** Maximum number of items that can be placed in this inventory*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItems;

	/**
	 * Maximum number of times an item is allowed to be stacked in a single inventory slot. 
	 * @note	Only inventory items that have 'bIsUnique' set to 'false' can be stacked.
	 *			If an inventory has 'bIsUnique' set to 'true', it cannot be stacked.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStackCount;

	void AddItem(FInventoryItem NewItem);
	void AddItem(FName ItemID);
	void AddItem(FName ItemID, FName ItemType, UDataTable* ItemLookupTable = nullptr);

	/** Removes a specified number of a specific item. Removes the entire stack */
	// void RemoveItem(FInventoryItem ItemToRemove, int32 ItemCount = 0);		// If ItemCount is <= 0 then all stacks of ItemToRemove are removed

	void RemoveItem(FName ItemID);
	void UseItem(FName ItemID);
	void UseItem(int32 ItemIndex);

	void SaveInventory();
	void LoadInventory();
	
	TArray<FInventoryItem> Items;

	TMap<FName, FInventorySlot> InventorySlots;

	/** Reduce item count by one in inventory */
	void ConsumeItem(TSubclassOf<UInventoryItemBase> ItemClass);


	///////////////////////////////////////////////////////////////////////////
	//  UI
public:

	inline void SetInventoryWidget(class UInventoryWidget* InWidget) { InventoryWidget = InWidget; }

	inline UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

protected:

	UPROPERTY(Transient)
	class UInventoryWidget* InventoryWidget;


	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddLoot(const FGeneratedLootInfo& LootInfo);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddInventoryItem(UObject* Item, int32 Count);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FInventorySlot& GetEmptySlot();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes)
	TSubclassOf<class UInventoryContainerWidget> InventoryContainerClass;

	UPROPERTY(Transient)
	TArray<FInventorySlot> Slots;

public:

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	///	New Inventory Code
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(UObject* ItemToAdd);

protected:

	FInventorySlot& GetNewInventorySlot();
	FInventorySlot& GetInventorySlotForItem(UObject* InventoryItem);
	
};
