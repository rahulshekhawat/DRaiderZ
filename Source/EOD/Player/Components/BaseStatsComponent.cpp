// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BaseStatsComponent.h"


// Sets default values for this component's properties
UBaseStatsComponent::UBaseStatsComponent()
{
	// Doesn't need to tick at the moment
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}

// Called every frame
void UBaseStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

// Called when the game starts
void UBaseStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
