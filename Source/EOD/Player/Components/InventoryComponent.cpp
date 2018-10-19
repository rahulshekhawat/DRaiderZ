// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryComponent.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUDWidget.h"
#include "UI/InventoryWidget.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This component doesn't tick
	PrimaryComponentTick.bCanEverTick = false;
	
	// Inventory doesn't need to be replicated
	SetIsReplicated(false);

	MaxSlots = 100;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::ToggleInventoryUI()
{
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!OwningPlayer)
	{
		return;
	}

	if (OwningPlayer->GetHUDWidget()->InventoryWidget->IsVisible())
	{
		OwningPlayer->GetHUDWidget()->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		OwningPlayer->GetHUDWidget()->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

FORCEINLINE UInventoryWidget* UInventoryComponent::GetInventoryWidget() const
{
	return InventoryWidget;
}

UInventoryWidget* UInventoryComponent::BP_GetInventoryWidget() const
{
	return GetInventoryWidget();
}

void UInventoryComponent::AddItem(FName ItemID)
{
	// @todo definition
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


