// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "EODLibrary.h"
#include "InventoryInterface.generated.h"

class AEODCharacterBase;

/** An interface that must be implemented for all in-game items that can be put in inventory */
UINTERFACE(BlueprintType)
class EOD_API UInventoryInterface : public UInterface
{
	GENERATED_BODY()

public:

    UInventoryInterface(const FObjectInitializer& ObjectInitializer);

};

/** An interface that must be implemented for all in-game items that can be put in inventory */
class EOD_API IInventoryInterface
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  Inventory Interface
	// --------------------------------------

	/** This event is called when the inventory item is clicked in inventory */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Inventory)
	void OnClick(AEODCharacterBase* ClickingChar) const;
	virtual void OnClick_Implementation(AEODCharacterBase* ClickingChar) const;

	virtual EInventoryItemType GetInventoryItemType() const;

	virtual FInGameInformation GetInGameInformation() const;

	/** 
	 * Returns the unique inventory item ID that is generated when the item is first added to the inventory.
	 * This ID is used to save the item information or meta data (e.g., whether the weapon in inventory is enchanted or not and so on)
	 * This function should NOT return NAME_None if the item has additional meta data that needs to be saved/loaded
	 */
	virtual FName GetInventoryItemID() const;

};
