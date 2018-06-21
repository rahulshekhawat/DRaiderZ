// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PrimeStatsComponent.h"

UPrimeStatsComponent::UPrimeStatsComponent(): Super()
{
	// Doesn't need to tick at the moment
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}

void UPrimeStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UPrimeStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}
