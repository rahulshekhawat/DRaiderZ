// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"

#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// class UTexture;
// class APlayerCharacter;
// class UInventoryWidget;

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

	/** Sets default values for this component's properties */
	UInventoryComponent(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts */
	virtual void BeginPlay() override;	

	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	TArray<FInventoryItem>& GetInventoryItems();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventoryItemChangedMCDelegate OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventoryItemChangedMCDelegate OnInventoryItemRemoved;

protected:

	/** Maximum number of inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots;

private:

	// void InitializeInventoryWidget();

	// FInventoryItem& GetItem(int32 ItemIndex) const;

	void AddItem(FName ItemID);

	void RemoveItem(FName ItemID);

	void UseItem(FName ItemID);

	void UseItem(int32 ItemIndex);

	void SaveInventory();

	void LoadInventory();
	
	TArray<FInventoryItem> Items;


};

/*
FORCEINLINE UInventoryWidget* UInventoryComponent::GetInventoryWidget() const
{
	return InventoryWidget;
}
*/
