// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Inventory doesn't need to be replicated
	bReplicates = false;

}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::ToggleInventoryUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory Toggled"));
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


