// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EOD_API IInventoryWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Widget")
	void InitializeInventory();
	virtual void InitializeInventory_Implementation() { ; }

};
