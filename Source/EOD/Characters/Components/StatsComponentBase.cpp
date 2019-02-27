// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatsComponentBase.h"

#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"

UStatsComponentBase::UStatsComponentBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This compnent doesn't tick
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	bHasHealthRegenration = false;
	bHasManaRegenration = false;
	bHasStaminaRegenration = false;

	LowHealthPercent = 0.15f;
	MovementSpeedModifier = 1.0f;
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
	DOREPLIFETIME(UStatsComponentBase, MovementSpeedModifier);

	DOREPLIFETIME_CONDITION(UStatsComponentBase, HealthRegenRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UStatsComponentBase, ManaRegenRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UStatsComponentBase, StaminaRegenRate, COND_OwnerOnly);

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

void UStatsComponentBase::OnRep_MaxHealth()
{
	OnHealthChanged.Broadcast(BaseHealth, MaxHealth, CurrentHealth);
}

void UStatsComponentBase::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(BaseHealth, MaxHealth, CurrentHealth);
}

void UStatsComponentBase::OnRep_MaxMana()
{
	OnManaChanged.Broadcast(BaseMana, MaxMana, CurrentMana);
}

void UStatsComponentBase::OnRep_CurrentMana()
{
	OnManaChanged.Broadcast(BaseMana, MaxMana, CurrentMana);
}

void UStatsComponentBase::OnRep_MaxStamina()
{
	OnStaminaChanged.Broadcast(BaseStamina, MaxStamina, CurrentStamina);
}

void UStatsComponentBase::OnRep_CurrentStamina()
{
	OnStaminaChanged.Broadcast(BaseStamina, MaxStamina, CurrentStamina);
}

void UStatsComponentBase::ActivateHealthRegeneration()
{
	if (IsValid(GetWorld()) && HealthRegenTickInterval > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle,
			this,
			&UStatsComponentBase::RegenerateHealth,
			HealthRegenTickInterval,
			true);
		bIsRegeneratingHealth = true;
	}
}

void UStatsComponentBase::ActivateManaRegeneration()
{
	if (IsValid(GetWorld()) && ManaRegenTickInterval > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ManaRegenTimerHandle,
			this,
			&UStatsComponentBase::RegenerateMana,
			ManaRegenTickInterval,
			true);
		bIsRegeneratingMana = true;
	}
}

void UStatsComponentBase::ActivateStaminaRegeneration()
{
	if (IsValid(GetWorld()) && StaminaRegenTickInterval > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle,
			this,
			&UStatsComponentBase::RegenerateStamina,
			StaminaRegenTickInterval,
			true);
		bIsRegeneratingStamina = true;
	}
}

void UStatsComponentBase::DeactivateHealthRegeneration()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
	}
	bIsRegeneratingHealth = false;
}

void UStatsComponentBase::DeactivateManaRegeneration()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(ManaRegenTimerHandle);
	}
	bIsRegeneratingMana = false;
}

void UStatsComponentBase::DeactivateStaminaRegeneration()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
	bIsRegeneratingStamina = false;
}

void UStatsComponentBase::RegenerateHealth()
{
	ModifyCurrentHealth(HealthRegenRate);
	if (CurrentHealth >= MaxHealth)
	{
		DeactivateHealthRegeneration();
	}
}

void UStatsComponentBase::RegenerateMana()
{
	ModifyCurrentMana(ManaRegenRate);
	if (CurrentMana >= MaxMana)
	{
		DeactivateManaRegeneration();
	}
}

void UStatsComponentBase::RegenerateStamina()
{
	ModifyCurrentStamina(StaminaRegenRate);
	if (CurrentStamina >= MaxStamina)
	{
		DeactivateStaminaRegeneration();
	}
}
