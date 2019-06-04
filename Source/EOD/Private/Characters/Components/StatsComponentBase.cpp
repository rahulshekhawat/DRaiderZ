// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatsComponentBase.h"
#include "EOD.h"

#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"

UStatsComponentBase::UStatsComponentBase(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer),
	Health(100, 100),
	Mana(100, 100),
	Stamina(100, 100),
	HealthRegenRate(10.f),
	ManaRegenRate(10.f),
	StaminaRegenRate(10.f)
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

	DOREPLIFETIME(UStatsComponentBase, Health);
	DOREPLIFETIME(UStatsComponentBase, Mana);
	DOREPLIFETIME(UStatsComponentBase, Stamina);


	DOREPLIFETIME_CONDITION(UStatsComponentBase, HealthRegenRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UStatsComponentBase, ManaRegenRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UStatsComponentBase, StaminaRegenRate, COND_OwnerOnly);
}

void UStatsComponentBase::BeginPlay()
{
	Super::BeginPlay();

	/**
	 * @note
	 *	When I bound these delegates in constructor of UStatsComponentBase,
	 *	GetWorld() always returned null in ActivateSTATRegen() function (which gets called through OnSTATChanged() function)
	 *	So I've added these delegate bindings in BeginPlay() and everything works fine now.
	 */		

	if (!Health.OnStatValueChanged.IsBoundToObject(this))
	{
		Health.OnStatValueChanged.AddUObject(this, &UStatsComponentBase::OnHealthChanged);
	}

	if (!Mana.OnStatValueChanged.IsBoundToObject(this))
	{
		Mana.OnStatValueChanged.AddUObject(this, &UStatsComponentBase::OnManaChanged);
	}

	if (!Stamina.OnStatValueChanged.IsBoundToObject(this))
	{
		Stamina.OnStatValueChanged.AddUObject(this, &UStatsComponentBase::OnStaminaChanged);
	}

	//~ @todo Load stat values
	/*
	//~ Initialize current variables
	SetMaxHealth(BaseHealth);
	SetCurrentHealth(BaseHealth);

	SetMaxMana(BaseMana);
	SetCurrentMana(BaseMana);

	SetMaxStamina(BaseStamina);
	SetCurrentStamina(BaseStamina);
	*/
}

void UStatsComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStatsComponentBase::OnRep_Health()
{
	Health.ForceBroadcastDelegate();
}

void UStatsComponentBase::OnRep_Mana()
{
	Mana.ForceBroadcastDelegate();
}

void UStatsComponentBase::OnRep_Stamina()
{
	Stamina.ForceBroadcastDelegate();
}

bool UStatsComponentBase::IsLowOnHealth()
{
	float CurrentHealth = (float)Health.GetCurrentValue();
	float MaxHealth = (float)Health.GetMaxValue();
	return (CurrentHealth / MaxHealth) <= LowHealthPercent;
}

void UStatsComponentBase::OnHealthChanged(int32 MaxValue, int32 CurrentValue)
{
	if (bHasHealthRegenration && CurrentValue < MaxValue && !bIsRegeneratingHealth)
	{
		ActivateHealthRegeneration();
	}
}

void UStatsComponentBase::OnManaChanged(int32 MaxValue, int32 CurrentValue)
{
	if (bHasManaRegenration && CurrentValue < MaxValue && !bIsRegeneratingMana)
	{
		ActivateManaRegeneration();
	}
}

void UStatsComponentBase::OnStaminaChanged(int32 MaxValue, int32 CurrentValue)
{
	if (bHasStaminaRegenration && CurrentValue < MaxValue && !bIsRegeneratingStamina)
	{
		ActivateStaminaRegeneration();
	}
}

void UStatsComponentBase::ActivateHealthRegeneration()
{
	UWorld* World = GetWorld();
	if (World && HealthRegenTickInterval > 0.f)
	{
		World->GetTimerManager().SetTimer(HealthRegenTimerHandle,
			this,
			&UStatsComponentBase::RegenerateHealth,
			HealthRegenTickInterval,
			true);
		bIsRegeneratingHealth = true;
	}
}

void UStatsComponentBase::ActivateManaRegeneration()
{
	UWorld* World = GetWorld();
	if (World && ManaRegenTickInterval > 0.f)
	{
		World->GetTimerManager().SetTimer(ManaRegenTimerHandle,
			this,
			&UStatsComponentBase::RegenerateMana,
			ManaRegenTickInterval,
			true);
		bIsRegeneratingMana = true;
	}
}

void UStatsComponentBase::ActivateStaminaRegeneration()
{
	UWorld* World = GetWorld();
	if (World && StaminaRegenTickInterval > 0.f)
	{
		World->GetTimerManager().SetTimer(StaminaRegenTimerHandle,
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
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(ManaRegenTimerHandle);
	}
	bIsRegeneratingMana = false;
}

void UStatsComponentBase::DeactivateStaminaRegeneration()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
	bIsRegeneratingStamina = false;
}

void UStatsComponentBase::RegenerateHealth()
{
	float RegenRate = HealthRegenRate.GetValue();
	Health.ModifyCurrentValue(RegenRate);
	int32 CurrentValue = Health.GetCurrentValue();
	int32 MaxValue = Health.GetMaxValue();

	if (CurrentValue >= MaxValue)
	{
		DeactivateHealthRegeneration();
	}
}

void UStatsComponentBase::RegenerateMana()
{
	float RegenRate = ManaRegenRate.GetValue();
	Mana.ModifyCurrentValue(RegenRate);
	int32 CurrentValue = Mana.GetCurrentValue();
	int32 MaxValue = Mana.GetMaxValue();

	if (CurrentValue >= MaxValue)
	{
		DeactivateManaRegeneration();
	}
}

void UStatsComponentBase::RegenerateStamina()
{
	float RegenRate = StaminaRegenRate.GetValue();
	Stamina.ModifyCurrentValue(RegenRate);
	int32 CurrentValue = Stamina.GetCurrentValue();
	int32 MaxValue = Stamina.GetMaxValue();

	if (CurrentValue >= MaxValue)
	{
		DeactivateStaminaRegeneration();
	}
}
