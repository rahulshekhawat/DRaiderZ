// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatsComponentBase.h"

#include "UnrealNetwork.h"

UStatsComponentBase::UStatsComponentBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This compnent doesn't tick
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	bHasHealthRegenration = false;
	bHasManaRegenration = false;
	bHasStaminaRegenration = false;

	LowHealthPercent = 0.15f;
}

void UStatsComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatsComponentBase, MaxHealth);
	DOREPLIFETIME(UStatsComponentBase, CurrentHealth);
	DOREPLIFETIME(UStatsComponentBase, MaxMana);
	DOREPLIFETIME(UStatsComponentBase, CurrentMana);
	DOREPLIFETIME(UStatsComponentBase, MaxStamina);
	DOREPLIFETIME(UStatsComponentBase, CurrentStamina);	
}

void UStatsComponentBase::BeginPlay()
{
	Super::BeginPlay();

	//~ Initialize current variables
	SetMaxHealth(BaseHealth);
	SetCurrentHealth(BaseHealth);

	SetMaxMana(BaseMana);
	SetCurrentMana(BaseMana);

	SetMaxStamina(BaseStamina);
	SetCurrentStamina(BaseStamina);

}

void UStatsComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
