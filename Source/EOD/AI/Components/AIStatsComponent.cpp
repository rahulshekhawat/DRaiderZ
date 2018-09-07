// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AIStatsComponent.h"

#include "UnrealNetwork.h"

UAIStatsComponent::UAIStatsComponent(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
}

void UAIStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAIStatsComponent, MaxHealth);
	DOREPLIFETIME(UAIStatsComponent, CurrentHealth);
	DOREPLIFETIME(UAIStatsComponent, MaxMana);
	DOREPLIFETIME(UAIStatsComponent, CurrentMana);

	DOREPLIFETIME(UAIStatsComponent, MovementSpeedModifier);
	DOREPLIFETIME(UAIStatsComponent, ActiveTimeDilation);
	DOREPLIFETIME(UAIStatsComponent, SpellCastingSpeedModifier);
	
	DOREPLIFETIME(UAIStatsComponent, Darkness);

}

int32 UAIStatsComponent::GetBaseHealth() const
{
	return BaseHealth;
}

int32 UAIStatsComponent::GetMaxHealth() const
{
	return MaxHealth;
}

int32 UAIStatsComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

int32 UAIStatsComponent::ModifyBaseHealth(int32 Value)
{
	BaseHealth += Value;
	return BaseHealth;
}

int32 UAIStatsComponent::ModifyMaxHealth(int32 Value)
{
	MaxHealth += Value;
	return MaxHealth;
}

int32 UAIStatsComponent::ModifyCurrentHealth(int32 Value)
{
	CurrentHealth += Value;
	return CurrentHealth;
}

bool UAIStatsComponent::IsLowOnHealth() const
{
	return (CurrentHealth / MaxHealth) <= 0.25;
}

int32 UAIStatsComponent::GetBaseMana() const
{
	return BaseMana;
}

int32 UAIStatsComponent::GetMaxMana() const
{
	return MaxMana;
}

int32 UAIStatsComponent::GetCurrentMana() const
{
	return CurrentMana;
}

int32 UAIStatsComponent::ModifyBaseMana(int32 Value)
{
	BaseMana += Value;
	return BaseMana;
}

int32 UAIStatsComponent::ModifyMaxMana(int32 Value)
{
	MaxMana += Value;
	return MaxMana;
}

int32 UAIStatsComponent::ModifyCurrentMana(int32 Value)
{
	CurrentMana += Value;
	return CurrentMana;
}

int32 UAIStatsComponent::GetBaseStamina() const
{
	// AI stats component does not implement BaseStamina
	return 0;
}

int32 UAIStatsComponent::GetMaxStamina() const
{
	// AI stats component does not implement MaxStamina
	return 0;
}

int32 UAIStatsComponent::GetCurrentStamina() const
{
	// AI stats component does not implement CurrentStamina
	return 0;
}

int32 UAIStatsComponent::ModifyBaseStamina(int32 Value)
{
	// empty definition
	return 0;
}

int32 UAIStatsComponent::ModifyMaxStamina(int32 Value)
{
	// empty definition
	return 0;
}

int32 UAIStatsComponent::ModifyCurrentStamina(int32 Value)
{
	// empty definition
	return 0;
}

int32 UAIStatsComponent::GetHealthRegenRate() const
{
	return HealthRegenRate;
}

int32 UAIStatsComponent::GetManaRegenRate() const
{
	return ManaRegenRate;
}

int32 UAIStatsComponent::GetStaminaRegenRate() const
{
	// AI stats component does not implement StaminaRegenRate
	return 0;
}

int32 UAIStatsComponent::GetPhysicalAttack() const
{
	return PhysicalAttack;
}

int32 UAIStatsComponent::GetMagickAttack() const
{
	return MagickAttack;
}

int32 UAIStatsComponent::GetPhysicalResistance() const
{
	return PhysicalResistance;
}

int32 UAIStatsComponent::GetMagickResistance() const
{
	return MagickResistance;
}

float UAIStatsComponent::GetPhysicalCritRate() const
{
	return PhysicalCritRate;
}

float UAIStatsComponent::GetMagickCritRate() const
{
	return MagickCritRate;
}

int32 UAIStatsComponent::GetPhysicalCritBonus() const
{
	return PhysicalCritBonus;
}

int32 UAIStatsComponent::GetMagickCritBonus() const
{
	return MagickCritBonus;
}

int32 UAIStatsComponent::GetElementalFireResistance() const
{
	return ElementalFireResistance;
}

int32 UAIStatsComponent::GetElementalIceResistance() const
{
	return ElementalIceResistance;
}

int32 UAIStatsComponent::ModifyElementalIceResistance(int32 Value)
{
	return ElementalIceResistance += Value;
}

int32 UAIStatsComponent::GetElementalLightningResistance() const
{
	return ElementalLightningResistance;
}

int32 UAIStatsComponent::GetElementalPoisonResistance() const
{
	return ElementalPoisonResistance;
}

int32 UAIStatsComponent::GetElementalHolyResistance() const
{
	return ElementalHolyResistance;
}

int32 UAIStatsComponent::GetElementalDarkResistance() const
{
	return ElementalDarkResistance;
}

int32 UAIStatsComponent::GetElementalFireDamage() const
{
	return ElementalFireDamage;
}

int32 UAIStatsComponent::GetElementalIceDamage() const
{
	return ElementalIceDamage;
}

int32 UAIStatsComponent::GetElementalLightningDamage() const
{
	return ElementalLightningDamage;
}

int32 UAIStatsComponent::GetElementalPoisonDamage() const
{
	return ElementalPoisonDamage;
}

int32 UAIStatsComponent::GetElementalHolyDamage() const
{
	return ElementalHolyDamage;
}

int32 UAIStatsComponent::GetElementalDarkDamage() const
{
	return ElementalDarkDamage;
}

int32 UAIStatsComponent::GetBleedResistance() const
{
	return BleedResistance;
}

int32 UAIStatsComponent::GetCrowdControlResistance() const
{
	return CrowdControlResistance;
}

float UAIStatsComponent::GetCooldownModifier() const
{
	return CooldownModifier;
}

float UAIStatsComponent::GetExpModifier() const
{
	return ExpModifier;
}

float UAIStatsComponent::GetDropRateModifier() const
{
	return DropRateModifier;
}

float UAIStatsComponent::GetStaminaConsumptionModifier() const
{
	return 1;
}

float UAIStatsComponent::GetMovementSpeedModifier() const
{
	return MovementSpeedModifier;
}

float UAIStatsComponent::GetActiveTimeDilation() const
{
	return ActiveTimeDilation;
}

float UAIStatsComponent::ModifyActiveTimeDilation(float Value)
{
	ActiveTimeDilation += Value;
	return ActiveTimeDilation;
}

float UAIStatsComponent::GetSpellCastingSpeedModifier() const
{
	return SpellCastingSpeedModifier;
}

int32 UAIStatsComponent::GetDarkness() const
{
	return Darkness;
}

