// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryComponent.h"
#include "EODPlayerController.h"
#include "InventoryWidget.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This component doesn't tick
	PrimaryComponentTick.bCanEverTick = false;

	// Inventory doesn't need to be replicated
	SetIsReplicated(false);

	MaxItems 				= 100;
	MaxStackCount			= 100;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<FInventoryItem>& UInventoryComponent::GetInventoryItems()
{
	return Items;
}

void UInventoryComponent::AddItem(FInventoryItem NewItem)
{
}

void UInventoryComponent::AddItem(FName ItemID)
{
	// @todo definition
}

void UInventoryComponent::AddItem(FName ItemID, FName ItemType, UDataTable* ItemLookupTable)
{
}

void UInventoryComponent::RemoveItem(FName ItemID)
{
	// @todo definition
}

void UInventoryComponent::UseItem(FName ItemID)
{
	// @todo definition
}

void UInventoryComponent::UseItem(int32 ItemIndex)
{
	// @todo definition
}

void UInventoryComponent::SaveInventory()
{
	// @todo definition
}

void UInventoryComponent::LoadInventory()
{
	// @todo definition
}


