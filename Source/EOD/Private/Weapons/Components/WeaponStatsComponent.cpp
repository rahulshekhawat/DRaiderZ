// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponStatsComponent.h"


// Sets default values for this component's properties
UWeaponStatsComponent::UWeaponStatsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// This component doesn't tick
	PrimaryComponentTick.bCanEverTick = false;

}

void UWeaponStatsComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UWeaponStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

