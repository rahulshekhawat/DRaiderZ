// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/InventoryItemBase.h"
#include "ConsumableBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UConsumableBase : public UInventoryItemBase
{
	GENERATED_BODY()
	
public:

	UConsumableBase(const FObjectInitializer& ObjectInitializer);

	virtual void OnClick() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Consumable")
	void Consume();
	virtual void Consume_Implementation();

};
