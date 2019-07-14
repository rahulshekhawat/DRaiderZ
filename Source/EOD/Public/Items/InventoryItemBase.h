// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemBase.generated.h"

class UTexture;

/**
 * 
 */
UCLASS()
class EOD_API UInventoryItemBase : public UObject
{
	GENERATED_BODY()
	
public:

	UInventoryItemBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItem")
	FText InGameName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItem")
	FText InGameDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItem")
	FName ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryItem")
	UTexture* Icon;

	/** Event called when player clicks on the inventory item */
	virtual void OnClick();

};
