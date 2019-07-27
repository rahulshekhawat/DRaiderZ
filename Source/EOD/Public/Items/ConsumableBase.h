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
	
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UConsumableBase(const FObjectInitializer& ObjectInitializer);


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void OnClick(class AEODPlayerController* InPC) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Consumable)
	bool Consume(class AEODPlayerController* InPC);
	virtual bool Consume_Implementation(class AEODPlayerController* InPC);

};
