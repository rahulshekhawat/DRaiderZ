// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This component doesn't tick
	PrimaryComponentTick.bCanEverTick = false;
	
	// Inventory doesn't need to be replicated
	SetIsReplicated(false);

	MaxSlots = 100;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::ToggleInventoryUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory Toggled"));
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


