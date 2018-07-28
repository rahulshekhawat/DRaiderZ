// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerStatsComponent.h"

#include "UnrealNetwork.h"

UPlayerStatsComponent::UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	AddCrowdControlImmunity(ECrowdControlEffect::Crystalized);
	RemoveCrowdControlImmunity(ECrowdControlEffect::Crystalized);
}

void UPlayerStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerStatsComponent, MaxHealth);
	DOREPLIFETIME(UPlayerStatsComponent, CurrentHealth);
	DOREPLIFETIME(UPlayerStatsComponent, MaxMana);
	DOREPLIFETIME(UPlayerStatsComponent, CurrentMana);
	DOREPLIFETIME(UPlayerStatsComponent, MaxStamina);
	DOREPLIFETIME(UPlayerStatsComponent, CurrentStamina);

	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, HealthRegenRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, ManaRegenRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, StaminaRegenRate, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, PhysicalAttack, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, MagickAttack, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, PhysicalResistance, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, MagickResistance, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, PhysicalCritRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, MagickCritRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, PhysicalCritBonus, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, MagickCritBonus, COND_OwnerOnly);

	DOREPLIFETIME(UPlayerStatsComponent, ElementalFireDamage);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalIceDamage);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalPoisonDamage);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalLightningDamage);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalHolyDamage);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalDarkDamage);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalFireResistance);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalIceResistance);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalPoisonResistance);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalLightningResistance);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalHolyResistance);
	DOREPLIFETIME(UPlayerStatsComponent, ElementalDarkResistance);
	
	DOREPLIFETIME(UPlayerStatsComponent, BleedResistance);
	DOREPLIFETIME(UPlayerStatsComponent, CrowdControlResistance);
	
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, CooldownModifier, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, ExpModifier, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, DropRateModifier, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPlayerStatsComponent, StaminaConsumptionModifier, COND_OwnerOnly);
	
	DOREPLIFETIME(UPlayerStatsComponent, MovementSpeedModifier);
	// DOREPLIFETIME(UPlayerStatsComponent, AnimationSpeedModifier);
	DOREPLIFETIME(UPlayerStatsComponent, ActiveTimeDilation);
	DOREPLIFETIME(UPlayerStatsComponent, SpellCastingSpeedModifier);
	
	DOREPLIFETIME(UPlayerStatsComponent, Darkness);

}

int32 UPlayerStatsComponent::GetBaseHealth() const
{
	return BaseHealth;
}

int32 UPlayerStatsComponent::GetMaxHealth() const
{
	return MaxHealth;
}

int32 UPlayerStatsComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UPlayerStatsComponent::ModifyBaseHealth(int32 Value)
{
	BaseHealth += Value;
}

void UPlayerStatsComponent::ModifyMaxHealth(int32 Value)
{
	MaxHealth += Value;
}

void UPlayerStatsComponent::ModifyCurrentHealth(int32 Value)
{
	CurrentHealth += Value;
}

bool UPlayerStatsComponent::IsLowOnHealth() const
{
	return (CurrentHealth / MaxHealth) <= 0.25;
}

int32 UPlayerStatsComponent::GetBaseMana() const
{
	return BaseMana;
}

int32 UPlayerStatsComponent::GetMaxMana() const
{
	return MaxMana;
}

int32 UPlayerStatsComponent::GetCurrentMana() const
{
	return CurrentMana;
}

void UPlayerStatsComponent::ModifyBaseMana(int32 Value)
{
	BaseMana += Value;
}

void UPlayerStatsComponent::ModifyMaxMana(int32 Value)
{
	MaxMana += Value;
}

void UPlayerStatsComponent::ModifyCurrentMana(int32 Value)
{
	CurrentMana += Value;
}

int32 UPlayerStatsComponent::GetBaseStamina() const
{
	return BaseStamina;
}

int32 UPlayerStatsComponent::GetMaxStamina() const
{
	return MaxStamina;
}

int32 UPlayerStatsComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

void UPlayerStatsComponent::ModifyBaseStamina(int32 Value)
{
	BaseStamina += Value;
}

void UPlayerStatsComponent::ModifyMaxStamina(int32 Value)
{
	MaxStamina += Value;
}

void UPlayerStatsComponent::ModifyCurrentStamina(int32 Value)
{
	CurrentStamina += Value;
}

int32 UPlayerStatsComponent::GetHealthRegenRate() const
{
	return HealthRegenRate;
}

int32 UPlayerStatsComponent::GetManaRegenRate() const
{
	return ManaRegenRate;
}

int32 UPlayerStatsComponent::GetStaminaRegenRate() const
{
	return StaminaRegenRate;
}

int32 UPlayerStatsComponent::GetPhysicalAttack() const
{
	return PhysicalAttack;
}

int32 UPlayerStatsComponent::GetMagickAttack() const
{
	return MagickAttack;
}

int32 UPlayerStatsComponent::GetPhysicalResistance() const
{
	return PhysicalResistance;
}

int32 UPlayerStatsComponent::GetMagickResistance() const
{
	return MagickResistance;
}

float UPlayerStatsComponent::GetPhysicalCritRate() const
{
	return PhysicalCritRate;
}

float UPlayerStatsComponent::GetMagickCritRate() const
{
	return MagickCritRate;
}

float UPlayerStatsComponent::GetPhysicalCritBonus() const
{
	return PhysicalCritBonus;
}

float UPlayerStatsComponent::GetMagickCritBonus() const
{
	return MagickCritBonus;
}

int32 UPlayerStatsComponent::GetElementalFireResistance() const
{
	return ElementalFireResistance;
}

int32 UPlayerStatsComponent::GetElementalIceResistance() const
{
	return ElementalIceResistance;
}

int32 UPlayerStatsComponent::ModifyElementalIceResistance(int32 Value)
{
	return ElementalIceResistance += Value;
}

int32 UPlayerStatsComponent::GetElementalLightningResistance() const
{
	return ElementalLightningResistance;
}

int32 UPlayerStatsComponent::GetElementalPoisonResistance() const
{
	return ElementalPoisonResistance;
}

int32 UPlayerStatsComponent::GetElementalHolyResistance() const
{
	return ElementalHolyResistance;
}

int32 UPlayerStatsComponent::GetElementalDarkResistance() const
{
	return ElementalDarkResistance;
}

int32 UPlayerStatsComponent::GetElementalFireDamage() const
{
	return ElementalFireDamage;
}

int32 UPlayerStatsComponent::GetElementalIceDamage() const
{
	return ElementalIceDamage;
}

int32 UPlayerStatsComponent::GetElementalLightningDamage() const
{
	return ElementalLightningDamage;
}

int32 UPlayerStatsComponent::GetElementalPoisonDamage() const
{
	return ElementalPoisonDamage;
}

int32 UPlayerStatsComponent::GetElementalHolyDamage() const
{
	return ElementalHolyDamage;
}

int32 UPlayerStatsComponent::GetElementalDarkDamage() const
{
	return ElementalDarkDamage;
}

int32 UPlayerStatsComponent::GetBleedResistance() const
{
	return BleedResistance;
}

int32 UPlayerStatsComponent::GetCrowdControlResistance() const
{
	return CrowdControlResistance;
}

void UPlayerStatsComponent::AddCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect)
{
	CrowdControlImmunities |= (1 << (uint8)CrowdControlEffect);
}

void UPlayerStatsComponent::AddCrowdControlImmunities(uint8 CrowdControlImmunities)
{
	this->CrowdControlImmunities |= CrowdControlImmunities;
}

void UPlayerStatsComponent::RemoveCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect)
{
	CrowdControlImmunities ^= (1 << (uint8)CrowdControlEffect);
}

void UPlayerStatsComponent::RemoveCrowdControlImmunities(uint8 CrowdControlImmunities)
{
	this->CrowdControlImmunities ^= CrowdControlImmunities;
}

void UPlayerStatsComponent::RemoveAllCrowdControlImmunities()
{
	CrowdControlImmunities = 0;
}

bool UPlayerStatsComponent::HasCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) const
{
	return (CrowdControlImmunities & (1 << (uint8)CrowdControlEffect));
}

uint8 UPlayerStatsComponent::GetCrowdControlImmunities() const
{
	return CrowdControlImmunities;
}

float UPlayerStatsComponent::GetCooldownModifier() const
{
	return CooldownModifier;
}

float UPlayerStatsComponent::GetExpModifier() const
{
	return ExpModifier;
}

float UPlayerStatsComponent::GetDropRateModifier() const
{
	return DropRateModifier;
}

float UPlayerStatsComponent::GetStaminaConsumptionModifier() const
{
	return StaminaConsumptionModifier;
}

float UPlayerStatsComponent::GetMovementSpeedModifier() const
{
	return MovementSpeedModifier;
}

float UPlayerStatsComponent::GetActiveTimeDilation() const
{
	return ActiveTimeDilation;
}

void UPlayerStatsComponent::ModifyActiveTimeDilation(float Value)
{
	ActiveTimeDilation += Value;
}

float UPlayerStatsComponent::GetSpellCastingSpeedModifier() const
{
	return SpellCastingSpeedModifier;
}

int32 UPlayerStatsComponent::GetDarkness() const
{
	return Darkness;
}
