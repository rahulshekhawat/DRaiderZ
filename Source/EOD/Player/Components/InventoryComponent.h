// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UTexture;
class APlayerCharacter;
class UInventoryWidget;

USTRUCT(BlueprintType)
struct EOD_API FInventoryItem
{
	GENERATED_USTRUCT_BODY()

public:

	FName ItemID;

	UTexture* ItemIcon;

	FString Description;

	// @todo hover info

};

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

	/** Called when the game starts */
	virtual void BeginPlay() override;	

	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Returns inventory widget */
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const;

	/** Returns inventory widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get Inventory Widget"))
	UInventoryWidget* BP_GetInventoryWidget() const;

	void InitializeComponentWidget();

	/** Toggle the display of inventory UI in player viewport */
	void ToggleInventoryUI();
	
private:

	UPROPERTY(Transient)
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	/** Maximum number of inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 MaxSlots;

	// FInventoryItem& GetItem(int32 ItemIndex) const;

	void AddItem(FName ItemID);

	void RemoveItem(FName ItemID);

	void UseItem(FName ItemID);

	void UseItem(int32 ItemIndex);

	void SaveInventory();

	void LoadInventory();
	
	TArray<FInventoryItem> Items;


};

FORCEINLINE UInventoryWidget* UInventoryComponent::GetInventoryWidget() const
{
	return InventoryWidget;
}
