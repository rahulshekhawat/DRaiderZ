// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatsComponentBase.h"


UStatsComponentBase::UStatsComponentBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This compnent doesn't tick
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

	bHasHealthRegenration = false;
	bHasManaRegenration = false;
	bHasStaminaRegenration = false;
}

void UStatsComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UStatsComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

