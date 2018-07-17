// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

/**
 * InventoryComponent is used to implement and manage player inventory
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	/** Sets default values for this component's properties */
	UInventoryComponent(const FObjectInitializer& ObjectInitializer);
	
	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Toggle the display of inventory UI in player viewport */
	void ToggleInventoryUI();

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

public:	
	
	/** Maximum number of inventory slots */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	uint32 MaxSlots;


	
};
