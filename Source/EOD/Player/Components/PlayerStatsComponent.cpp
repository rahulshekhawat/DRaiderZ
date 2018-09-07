// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerStatsComponent.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUDWidget.h"

#include "UnrealNetwork.h"
#include "Components/ProgressBar.h"

UPlayerStatsComponent::UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	AddCrowdControlImmunity(ECrowdControlEffect::Crystalized);
	RemoveCrowdControlImmunity(ECrowdControlEffect::Crystalized);

	MovementSpeedModifier = 1.f;
	StaminaConsumptionModifier = 1.f;

	OwningPlayer = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerStatsComponent::PostInitProperties()
{
	Super::PostInitProperties();

	
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	//~ Initialize current variables
	ModifyMaxHealth(BaseHealth);
	ModifyCurrentHealth(BaseHealth);

	ModifyMaxMana(BaseMana);
	ModifyCurrentMana(BaseMana);

	ModifyMaxStamina(BaseStamina);
	ModifyCurrentStamina(BaseStamina);

	// @todo load damage and resistance stats

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

int32 UPlayerStatsComponent::ModifyBaseHealth(int32 Value)
{
	BaseHealth += Value;
	return BaseHealth;
}

int32 UPlayerStatsComponent::ModifyMaxHealth(int32 Value)
{
	MaxHealth += Value;
	return MaxHealth;
}

int32 UPlayerStatsComponent::ModifyCurrentHealth(int32 Value)
{
	// SetCurrentHealth()
	CurrentHealth += Value;
	// UpdatePlayerHealthBar();
	return CurrentHealth;
}

void UPlayerStatsComponent::SetBaseHealth(int32 Value)
{
}

void UPlayerStatsComponent::SetMaxHealth(int32 Value)
{
}

void UPlayerStatsComponent::SetCurrentHealth(int32 Value)
{
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

int32 UPlayerStatsComponent::ModifyBaseMana(int32 Value)
{
	BaseMana += Value;
	return BaseMana;
}

int32 UPlayerStatsComponent::ModifyMaxMana(int32 Value)
{
	MaxMana += Value;
	return MaxMana;
}

int32 UPlayerStatsComponent::ModifyCurrentMana(int32 Value)
{
	CurrentMana += Value;

	// Only local player will have HUDWidget
	if (OwningPlayer && OwningPlayer->GetHUDWidget() && MaxMana != 0)
	{
		float Percent = (float)CurrentMana / (float)MaxMana;
		OwningPlayer->GetHUDWidget()->ManaBar->SetPercent(Percent);
	}

	return CurrentMana;
}

void UPlayerStatsComponent::SetBaseMana(int32 Value)
{
}

void UPlayerStatsComponent::SetMaxMana(int32 Value)
{
}

void UPlayerStatsComponent::SetCurrentMana(int32 Value)
{
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

int32 UPlayerStatsComponent::ModifyBaseStamina(int32 Value)
{
	BaseStamina += Value;
	return BaseStamina;
}

int32 UPlayerStatsComponent::ModifyMaxStamina(int32 Value)
{
	MaxStamina += Value;
	return MaxStamina;
}

int32 UPlayerStatsComponent::ModifyCurrentStamina(int32 Value)
{
	CurrentStamina += Value;

	// Only local player will have HUDWidget
	if (OwningPlayer && OwningPlayer->GetHUDWidget() && MaxStamina != 0)
	{
		float Percent = (float)CurrentStamina / (float)MaxStamina;
		OwningPlayer->GetHUDWidget()->StaminaBar->SetPercent(Percent);
	}

	return CurrentStamina;
}

void UPlayerStatsComponent::SetBaseStamina(int32 Value)
{
}

void UPlayerStatsComponent::SetMaxStamina(int32 Value)
{
}

void UPlayerStatsComponent::SetCurrentStamina(int32 Value)
{
}

int32 UPlayerStatsComponent::GetHealthRegenRate() const
{
	return HealthRegenRate;
}

int32 UPlayerStatsComponent::ModifyHealthRegenRate(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetHealthRegenRate(int32 Value)
{
}

int32 UPlayerStatsComponent::GetManaRegenRate() const
{
	return ManaRegenRate;
}

int32 UPlayerStatsComponent::ModifyManaRegenRate(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetManaRegenRate(int32 Value)
{
}

int32 UPlayerStatsComponent::GetStaminaRegenRate() const
{
	return StaminaRegenRate;
}

int32 UPlayerStatsComponent::ModifyStaminaRegenRate(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetStaminaRegenRate(int32 Value)
{
}

int32 UPlayerStatsComponent::GetPhysicalAttack() const
{
	return PhysicalAttack;
}

int32 UPlayerStatsComponent::GetMagickAttack() const
{
	return MagickAttack;
}

int32 UPlayerStatsComponent::ModifyPhysicalAttack(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyMagickAttack(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetPhysicalAttack(int32 Value)
{
}

void UPlayerStatsComponent::SetMagickAttack(int32 Value)
{
}

int32 UPlayerStatsComponent::GetPhysicalResistance() const
{
	return PhysicalResistance;
}

int32 UPlayerStatsComponent::GetMagickResistance() const
{
	return MagickResistance;
}

int32 UPlayerStatsComponent::ModifyPhysicalResistance(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyMagickResistance(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetPhysicalResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetMagickResistance(int32 Value)
{
}

float UPlayerStatsComponent::GetPhysicalCritRate() const
{
	return PhysicalCritRate;
}

float UPlayerStatsComponent::GetMagickCritRate() const
{
	return MagickCritRate;
}

float UPlayerStatsComponent::ModifyPhysicalCritRate(float Value)
{
	return 0.0f;
}

float UPlayerStatsComponent::ModifyMagickCritRate(float Value)
{
	return 0.0f;
}

void UPlayerStatsComponent::SetPhysicalCritRate(float Value)
{
}

void UPlayerStatsComponent::SetMagickCritRate(float Value)
{
}

int32 UPlayerStatsComponent::GetPhysicalCritBonus() const
{
	return PhysicalCritBonus;
}

int32 UPlayerStatsComponent::GetMagickCritBonus() const
{
	return MagickCritBonus;
}

int32 UPlayerStatsComponent::ModifyPhysicalCritBonus(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyMagickCritBonus(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetPhysicalCritBonus(int32 Value)
{
}

void UPlayerStatsComponent::SetMagickCritBonus(int32 Value)
{
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

int32 UPlayerStatsComponent::ModifyElementalLightningResistance(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalPoisonResistance(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalHolyResistance(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalDarkResistance(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetElementalFireResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalIceResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalLightningResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalPoisonResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalHolyResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalDarkResistance(int32 Value)
{
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

int32 UPlayerStatsComponent::ModifyElementalFireResistance(int32 Value)
{
	return int32();
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

int32 UPlayerStatsComponent::ModifyElementalFireDamage(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalIceDamage(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalLightningDamage(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalPoisonDamage(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalHolyDamage(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyElementalDarkDamage(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetElementalFireDamage(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalIceDamage(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalLightningDamage(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalPoisonDamage(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalHolyDamage(int32 Value)
{
}

void UPlayerStatsComponent::SetElementalDarkDamage(int32 Value)
{
}

int32 UPlayerStatsComponent::GetBleedResistance() const
{
	return BleedResistance;
}

int32 UPlayerStatsComponent::GetCrowdControlResistance() const
{
	return CrowdControlResistance;
}

int32 UPlayerStatsComponent::ModifyBleedResistance(int32 Value)
{
	return int32();
}

int32 UPlayerStatsComponent::ModifyCrowdControlResistance(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetBleedResistance(int32 Value)
{
}

void UPlayerStatsComponent::SetCrowdControlResistance(int32 Value)
{
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

float UPlayerStatsComponent::ModifyActiveTimeDilation(float Value)
{
	ActiveTimeDilation += Value;
	return ActiveTimeDilation;
}

float UPlayerStatsComponent::ModifySpellCastingSpeedModifier(float Value)
{
	return 0.0f;
}

void UPlayerStatsComponent::SetCooldownModifier(float Value)
{
}

void UPlayerStatsComponent::SetExpModifier(float Value)
{
}

void UPlayerStatsComponent::SetDropRateModifier(float Value)
{
}

void UPlayerStatsComponent::SetStaminaConsumptionModifier(float Value)
{
}

void UPlayerStatsComponent::SetMovementSpeedModifier(float Value)
{
}

void UPlayerStatsComponent::SetActiveTimeDilation(float Value)
{
}

void UPlayerStatsComponent::SetSpellCastingSpeedModifier(float Value)
{
}

float UPlayerStatsComponent::GetSpellCastingSpeedModifier() const
{
	return SpellCastingSpeedModifier;
}

float UPlayerStatsComponent::ModifyCooldownModifier(float Value)
{
	return 0.0f;
}

float UPlayerStatsComponent::ModifyExpModifier(float Value)
{
	return 0.0f;
}

float UPlayerStatsComponent::ModifyDropRateModifier(float Value)
{
	return 0.0f;
}

float UPlayerStatsComponent::ModifyStaminaConsumptionModifier(float Value)
{
	return 0.0f;
}

float UPlayerStatsComponent::ModifyMovementSpeedModifier(float Value)
{
	return 0.0f;
}

int32 UPlayerStatsComponent::GetDarkness() const
{
	return Darkness;
}

int32 UPlayerStatsComponent::ModifyDarkness(int32 Value)
{
	return int32();
}

void UPlayerStatsComponent::SetDarkness(int32 Value)
{
}

/*
FORCEINLINE void UPlayerStatsComponent::UpdatePlayerHealthBar() const
{
	// Only local player will have HUDWidget
	if (OwningPlayer && OwningPlayer->GetHUDWidget() && MaxHealth != 0)
	{
		float Percent = (float)CurrentHealth / (float)MaxHealth;
		OwningPlayer->GetHUDWidget()->HealthBar->SetPercent(Percent);
	}
}
*/
