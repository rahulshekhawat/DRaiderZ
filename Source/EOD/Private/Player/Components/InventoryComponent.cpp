// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryComponent.h"
#include "EODPlayerController.h"
#include "InventoryWidget.h"
#include "InventoryItemBase.h"
#include "InventoryContainerWidget.h"
#include "InventoryInterface.h"

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

const TArray<FInventoryItem>& UInventoryComponent::GetInventoryItems()
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

void UInventoryComponent::ConsumeItem(TSubclassOf<UInventoryItemBase> ItemClass)
{
}

void UInventoryComponent::AddLoot(const FGeneratedLootInfo& LootInfo)
{
	int32 SlotIndex = -1;

	for (int i = 0; i < Slots.Num(); i++)
	{
		FInventorySlot& Slot = Slots[i];
		if (Slot.ItemInSlot && Slot.ItemInSlot->GetClass() == LootInfo.ItemClass.Get())
		{
			Slot.ItemStackCount += LootInfo.ItemCount;
			if (Slot.SlotWidget)
			{
				Slot.SlotWidget->SetItemCount(Slot.ItemStackCount);
			}
			return;
		}
	}

	UObject* InventoryItem = NewObject<UObject>(this, LootInfo.ItemClass, NAME_None, RF_Transient);
	if (InventoryItem)
	{
		AddInventoryItem(InventoryItem, LootInfo.ItemCount);
	}
}

void UInventoryComponent::AddInventoryItem(UObject* Item, int32 Count)
{
	IInventoryInterface* InvItem = Cast<IInventoryInterface>(Item);
	if (InvItem == nullptr || Count <= 0)
	{
		return;
	}

	/*
	FInventorySlot& Slot = GetEmptySlot();
	Slot.ItemInSlot = Item;
	Slot.ItemStackCount = Count;
	if (Slot.SlotWidget)
	{
		Slot.SlotWidget->SetDataObj(Item);
		Slot.SlotWidget->SetItemCount(Count);
	}
	*/
}

FInventorySlot& UInventoryComponent::GetEmptySlot()
{
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.IsEmpty())
		{
			return Slot;
		}
	}

	FInventorySlot NewSlot;
	int32 SlotIndex = Slots.Add(NewSlot);
	FInventorySlot& SlotRef = Slots[SlotIndex];
	SlotRef.SlotIndex = SlotIndex;
	SlotRef.ItemStackCount = 0;

	AEODPlayerController* OwnerPC = Cast<AEODPlayerController>(GetOwner());
	if (OwnerPC && OwnerPC->IsLocalPlayerController())
	{
		UInventoryContainerWidget* InvContainer = CreateWidget<UInventoryContainerWidget>(OwnerPC, InventoryContainerClass.Get());
		UInventoryWidget* InvWidget = InventoryWidget ? InventoryWidget : OwnerPC->GetInventoryWidget();
		check(InvWidget);
		InvWidget->AddContainer(InvContainer);
		SlotRef.SlotWidget = InvContainer;		
	}

	return SlotRef;
}
