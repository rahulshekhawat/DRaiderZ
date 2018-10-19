// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerStatsComponent.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUDWidget.h"

#include "UnrealNetwork.h"
#include "Components/ProgressBar.h"

UPlayerStatsComponent::UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// AddCrowdControlImmunity(ECrowdControlEffect::Crystalized);
	// RemoveCrowdControlImmunity(ECrowdControlEffect::Crystalized);

	MovementSpeedModifier			= 1.f;
	StaminaConsumptionModifier		= 1.f;

	bHasHealthRegenration			= true;
	bHasManaRegenration				= true;
	bHasStaminaRegenration			= true;

	HealthRegenRate					= 10;
	ManaRegenRate					= 10;
	StaminaRegenRate				= 10;

	HealthRegenTickInterval			= 1.f;
	ManaRegenTickInterval			= 1.f;
	StaminaRegenTickInterval		= 1.f;

	ActiveTimeDilation = 1.f;

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
	SetMaxHealth(BaseHealth);
	SetCurrentHealth(BaseHealth);
	
	SetMaxMana(BaseMana);
	SetCurrentMana(BaseMana);

	SetMaxStamina(BaseStamina);
	SetCurrentStamina(BaseStamina);

	// @todo load damage and resistance stats

	ActivateHealthRegeneration();
	ActivateManaRegeneration();
	ActivateStaminaRegeneration();

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
	int32 Result = BaseHealth + Value;
	Result = Result <= 0 ? 0 : Result;
	SetBaseHealth(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyMaxHealth(int32 Value)
{
	int32 Result = MaxHealth + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMaxHealth(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyCurrentHealth(int32 Value)
{
	int32 Result = CurrentHealth + Value;
	if (Result < 0)
	{
		Result = 0;
	}
	else if (Result > MaxHealth)
	{
		Result = MaxHealth;
	}

	SetCurrentHealth(Result);
	return Result;
}

void UPlayerStatsComponent::SetBaseHealth(int32 Value)
{
	BaseHealth = Value;
}

void UPlayerStatsComponent::SetMaxHealth(int32 Value)
{
	MaxHealth = Value;
	//~ @todo update health bar UI
}

void UPlayerStatsComponent::SetCurrentHealth(int32 Value)
{
	CurrentHealth = Value;

	// Only local player will have HUDWidget
	if (OwningPlayer && OwningPlayer->GetHUDWidget())
	{
		OwningPlayer->GetHUDWidget()->UpdateHealthBar(CurrentHealth, MaxHealth, BaseHealth);
	}

	if (CurrentHealth < MaxHealth && !bIsRegeneratingHealth)
	{
		ActivateHealthRegeneration();
	}
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
	int32 Result = BaseMana + Value;
	Result = Result <= 0 ? 0 : Result;
	SetBaseMana(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyMaxMana(int32 Value)
{
	int32 Result = MaxMana + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMaxMana(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyCurrentMana(int32 Value)
{
	int32 Result = CurrentMana + Value;
	if (Result < 0)
	{
		Result = 0;
	}
	else if (Result > MaxMana)
	{
		Result = MaxMana;
	}

	SetCurrentMana(Result);
	return Result;
}

void UPlayerStatsComponent::SetBaseMana(int32 Value)
{
	BaseMana = Value;
}

void UPlayerStatsComponent::SetMaxMana(int32 Value)
{
	MaxMana = Value;
}

void UPlayerStatsComponent::SetCurrentMana(int32 Value)
{
	CurrentMana = Value;

	// Only local player will have HUDWidget
	if (OwningPlayer && OwningPlayer->GetHUDWidget())
	{
		OwningPlayer->GetHUDWidget()->UpdateManaBar(CurrentMana, MaxMana, BaseMana);
	}

	if (CurrentMana < MaxMana && !bIsRegeneratingMana)
	{
		ActivateManaRegeneration();
	}
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
	int32 Result = BaseStamina + Value;
	Result = Result <= 0 ? 0 : Result;
	SetBaseStamina(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyMaxStamina(int32 Value)
{
	int32 Result = MaxStamina + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMaxStamina(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyCurrentStamina(int32 Value)
{
	int32 Result = CurrentStamina + Value;
	if (Result < 0)
	{
		Result = 0;
	}
	else if (Result > MaxStamina)
	{
		Result = MaxStamina;
	}

	SetCurrentStamina(Result);
	return Result;
}

void UPlayerStatsComponent::SetBaseStamina(int32 Value)
{
	BaseStamina = Value;
}

void UPlayerStatsComponent::SetMaxStamina(int32 Value)
{
	MaxStamina = Value;
}

void UPlayerStatsComponent::SetCurrentStamina(int32 Value)
{
	CurrentStamina = Value;

	// Only local player will have HUDWidget
	if (OwningPlayer && OwningPlayer->GetHUDWidget())
	{
		OwningPlayer->GetHUDWidget()->UpdateStaminaBar(CurrentStamina, MaxStamina, BaseStamina);
	}

	if (CurrentStamina < MaxStamina && !bIsRegeneratingStamina)
	{
		ActivateStaminaRegeneration();
	}
}

int32 UPlayerStatsComponent::GetHealthRegenRate() const
{
	return HealthRegenRate;
}

int32 UPlayerStatsComponent::ModifyHealthRegenRate(int32 Value)
{
	int32 Result = HealthRegenRate + Value;
	// Result = Result <= 0 ? 0 : Result; // Health regen rate can be negative
	SetHealthRegenRate(Result);
	return Result;
}

void UPlayerStatsComponent::SetHealthRegenRate(int32 Value)
{
	HealthRegenRate = Value;
}

int32 UPlayerStatsComponent::GetManaRegenRate() const
{
	return ManaRegenRate;
}

int32 UPlayerStatsComponent::ModifyManaRegenRate(int32 Value)
{
	int32 Result = ManaRegenRate + Value;
	// Result = Result <= 0 ? 0 : Result; // Mana regen rate can be negative
	SetManaRegenRate(Result);
	return Result;
}

void UPlayerStatsComponent::SetManaRegenRate(int32 Value)
{
	ManaRegenRate = Value;
}

int32 UPlayerStatsComponent::GetStaminaRegenRate() const
{
	return StaminaRegenRate;
}

int32 UPlayerStatsComponent::ModifyStaminaRegenRate(int32 Value)
{
	int32 Result = StaminaRegenRate + Value;
	// Result = Result <= 0 ? 0 : Result; // Stamina regen rate can be negative
	SetStaminaRegenRate(Result);
	return Result;
}

void UPlayerStatsComponent::SetStaminaRegenRate(int32 Value)
{
	StaminaRegenRate = Value;
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
	int32 Result = PhysicalAttack + Value;
	Result = Result <= 0 ? 0 : Result;
	SetPhysicalAttack(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyMagickAttack(int32 Value)
{
	int32 Result = MagickAttack + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMagickAttack(Result);
	return Result;
}

void UPlayerStatsComponent::SetPhysicalAttack(int32 Value)
{
	PhysicalAttack = Value;
}

void UPlayerStatsComponent::SetMagickAttack(int32 Value)
{
	MagickAttack = Value;
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
	int32 Result = PhysicalResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetPhysicalResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyMagickResistance(int32 Value)
{
	int32 Result = MagickResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMagickResistance(Result);
	return Result;
}

void UPlayerStatsComponent::SetPhysicalResistance(int32 Value)
{
	PhysicalResistance = Value;
}

void UPlayerStatsComponent::SetMagickResistance(int32 Value)
{
	MagickResistance = Value;
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
	float Result = PhysicalCritRate + Value;
	Result = Result <= 0 ? 0 : Result;
	SetPhysicalCritRate(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyMagickCritRate(float Value)
{
	float Result = MagickCritRate + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMagickCritRate(Result);
	return Result;
}

void UPlayerStatsComponent::SetPhysicalCritRate(float Value)
{
	PhysicalCritRate = Value;
}

void UPlayerStatsComponent::SetMagickCritRate(float Value)
{
	MagickCritRate = Value;
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
	int32 Result = PhysicalCritBonus + Value;
	Result = Result <= 0 ? 0 : Result;
	SetPhysicalCritBonus(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyMagickCritBonus(int32 Value)
{
	int32 Result = MagickCritBonus + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMagickCritBonus(Result);
	return Result;
}

void UPlayerStatsComponent::SetPhysicalCritBonus(int32 Value)
{
	PhysicalCritBonus = Value;
}

void UPlayerStatsComponent::SetMagickCritBonus(int32 Value)
{
	MagickCritBonus = Value;
}

int32 UPlayerStatsComponent::GetElementalFireResistance() const
{
	return ElementalFireResistance;
}

int32 UPlayerStatsComponent::GetElementalIceResistance() const
{
	return ElementalIceResistance;
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
	int32 Result = ElementalFireResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalFireResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalIceResistance(int32 Value)
{
	int32 Result = ElementalIceResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalIceResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalLightningResistance(int32 Value)
{
	int32 Result = ElementalLightningResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalLightningResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalPoisonResistance(int32 Value)
{
	int32 Result = ElementalPoisonResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalPoisonResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalHolyResistance(int32 Value)
{
	int32 Result = ElementalHolyResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalHolyResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalDarkResistance(int32 Value)
{
	int32 Result = ElementalDarkResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalDarkResistance(Result);
	return Result;
}

void UPlayerStatsComponent::SetElementalFireResistance(int32 Value)
{
	ElementalFireResistance = Value;
}

void UPlayerStatsComponent::SetElementalIceResistance(int32 Value)
{
	ElementalIceResistance = Value;
}

void UPlayerStatsComponent::SetElementalLightningResistance(int32 Value)
{
	ElementalLightningResistance = Value;
}

void UPlayerStatsComponent::SetElementalPoisonResistance(int32 Value)
{
	ElementalPoisonResistance = Value;
}

void UPlayerStatsComponent::SetElementalHolyResistance(int32 Value)
{
	ElementalHolyResistance = Value;
}

void UPlayerStatsComponent::SetElementalDarkResistance(int32 Value)
{
	ElementalDarkResistance = Value;
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
	int32 Result = ElementalFireDamage + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalFireDamage(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalIceDamage(int32 Value)
{
	int32 Result = ElementalIceDamage + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalIceDamage(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalLightningDamage(int32 Value)
{
	int32 Result = ElementalLightningDamage + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalLightningDamage(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalPoisonDamage(int32 Value)
{
	int32 Result = ElementalPoisonDamage + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalPoisonDamage(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalHolyDamage(int32 Value)
{
	int32 Result = ElementalHolyDamage + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalHolyDamage(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyElementalDarkDamage(int32 Value)
{
	int32 Result = ElementalDarkDamage + Value;
	Result = Result <= 0 ? 0 : Result;
	SetElementalDarkDamage(Result);
	return Result;
}

void UPlayerStatsComponent::SetElementalFireDamage(int32 Value)
{
	ElementalFireDamage = Value;
}

void UPlayerStatsComponent::SetElementalIceDamage(int32 Value)
{
	ElementalIceDamage = Value;
}

void UPlayerStatsComponent::SetElementalLightningDamage(int32 Value)
{
	ElementalLightningDamage = Value;
}

void UPlayerStatsComponent::SetElementalPoisonDamage(int32 Value)
{
	ElementalPoisonDamage = Value;
}

void UPlayerStatsComponent::SetElementalHolyDamage(int32 Value)
{
	ElementalHolyDamage = Value;
}

void UPlayerStatsComponent::SetElementalDarkDamage(int32 Value)
{
	ElementalDarkDamage = Value;
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
	int32 Result = BleedResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetBleedResistance(Result);
	return Result;
}

int32 UPlayerStatsComponent::ModifyCrowdControlResistance(int32 Value)
{
	int32 Result = CrowdControlResistance + Value;
	Result = Result <= 0 ? 0 : Result;
	SetCrowdControlResistance(Result);
	return Result;
}

void UPlayerStatsComponent::SetBleedResistance(int32 Value)
{
	BleedResistance = Value;
}

void UPlayerStatsComponent::SetCrowdControlResistance(int32 Value)
{
	CrowdControlResistance = Value;
}

void UPlayerStatsComponent::AddCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect)
{
	CrowdControlImmunities |= (1 << (uint8)CrowdControlEffect);
}

void UPlayerStatsComponent::AddCrowdControlImmunities(uint8 CCImmunities)
{
	this->CrowdControlImmunities |= CrowdControlImmunities;
}

void UPlayerStatsComponent::RemoveCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect)
{
	CrowdControlImmunities ^= (1 << (uint8)CrowdControlEffect);
}

void UPlayerStatsComponent::RemoveCrowdControlImmunities(uint8 CCImmunities)
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

float UPlayerStatsComponent::GetSpellCastingSpeedModifier() const
{
	return SpellCastingSpeedModifier;
}

float UPlayerStatsComponent::ModifyCooldownModifier(float Value)
{
	float Result = CooldownModifier + Value;
	Result = Result <= 0 ? 0 : Result;
	SetCooldownModifier(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyExpModifier(float Value)
{
	float Result = ExpModifier + Value;
	Result = Result <= 0 ? 0 : Result;
	SetExpModifier(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyDropRateModifier(float Value)
{
	float Result = DropRateModifier + Value;
	Result = Result <= 0 ? 0 : Result;
	SetDropRateModifier(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyStaminaConsumptionModifier(float Value)
{
	float Result = StaminaConsumptionModifier + Value;
	Result = Result <= 0 ? 0 : Result;
	SetStaminaConsumptionModifier(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyMovementSpeedModifier(float Value)
{
	float Result = MovementSpeedModifier + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMovementSpeedModifier(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyActiveTimeDilation(float Value)
{
	float Result = ActiveTimeDilation + Value;
	Result = Result <= 0 ? 0 : Result;
	SetActiveTimeDilation(Result);
	return Result;
}

float UPlayerStatsComponent::ModifySpellCastingSpeedModifier(float Value)
{
	float Result = SpellCastingSpeedModifier + Value;
	Result = Result <= 0 ? 0 : Result;
	SetSpellCastingSpeedModifier(Result);
	return Result;
}

void UPlayerStatsComponent::SetCooldownModifier(float Value)
{
	CooldownModifier = Value;
}

void UPlayerStatsComponent::SetExpModifier(float Value)
{
	ExpModifier = Value;
}

void UPlayerStatsComponent::SetDropRateModifier(float Value)
{
	DropRateModifier = Value;
}

void UPlayerStatsComponent::SetStaminaConsumptionModifier(float Value)
{
	StaminaConsumptionModifier = Value;
}

void UPlayerStatsComponent::SetMovementSpeedModifier(float Value)
{
	MovementSpeedModifier = Value;
}

void UPlayerStatsComponent::SetActiveTimeDilation(float Value)
{
	ActiveTimeDilation = Value;
}

void UPlayerStatsComponent::SetSpellCastingSpeedModifier(float Value)
{
	SpellCastingSpeedModifier = Value;
}

int32 UPlayerStatsComponent::GetDarkness() const
{
	return Darkness;
}

int32 UPlayerStatsComponent::ModifyDarkness(int32 Value)
{
	int32 Result = Darkness + Value;
	Result = Result <= 0 ? 0 : Result;
	SetDarkness(Result);
	return Result;
}

void UPlayerStatsComponent::SetDarkness(int32 Value)
{
	Darkness = Value;
}

float UPlayerStatsComponent::GetPhysicalDamageReductionOnBlock() const
{
	return PhysicalDamageReductionOnBlock;
}

float UPlayerStatsComponent::GetMagickDamageReductionOnBlock() const
{
	return MagickDamageReductionOnBlock;
}

float UPlayerStatsComponent::ModifyPhysicalDamageReductionOnBlock(float Value)
{
	float Result = PhysicalDamageReductionOnBlock + Value;
	Result = Result <= 0 ? 0 : Result;
	SetPhysicalDamageReductionOnBlock(Result);
	return Result;
}

float UPlayerStatsComponent::ModifyMagickDamageReductionOnBlock(float Value)
{
	float Result = MagickDamageReductionOnBlock + Value;
	Result = Result <= 0 ? 0 : Result;
	SetMagickDamageReductionOnBlock(Result);
	return Result;
}

void UPlayerStatsComponent::SetPhysicalDamageReductionOnBlock(float Value)
{
	PhysicalDamageReductionOnBlock = Value;
}

void UPlayerStatsComponent::SetMagickDamageReductionOnBlock(float Value)
{
	MagickDamageReductionOnBlock = Value;
}

void UPlayerStatsComponent::InitializeComponentWidget()
{
	/*
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!(OwningPlayer && OwningPlayer->IsLocallyControlled() && OwningPlayer->GetHUDWidget()))
	{
		return;
	}

	if (SkillBarWidgetClass.Get())
	{
		SkillBarWidget = CreateWidget<USkillBarWidget>(OwningPlayer->GetGameInstance(), SkillBarWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillBarWidget(SkillBarWidget);
	}
	*/
}

void UPlayerStatsComponent::ActivateHealthRegeneration()
{
	GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle,
		this,
		&UPlayerStatsComponent::RegenerateHealth,
		HealthRegenTickInterval,
		true);
	bIsRegeneratingHealth = true;
}

void UPlayerStatsComponent::ActivateManaRegeneration()
{
	GetWorld()->GetTimerManager().SetTimer(ManaRegenTimerHandle,
		this,
		&UPlayerStatsComponent::RegenerateMana,
		ManaRegenTickInterval,
		true);
	bIsRegeneratingMana = true;
}

void UPlayerStatsComponent::ActivateStaminaRegeneration()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle,
		this,
		&UPlayerStatsComponent::RegenerateStamina,
		StaminaRegenTickInterval,
		true);
	bIsRegeneratingStamina = true;
}

void UPlayerStatsComponent::DeactivateHealthRegeneration()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
	bIsRegeneratingHealth = false;
}

void UPlayerStatsComponent::DeactivateManaRegeneration()
{
	GetWorld()->GetTimerManager().ClearTimer(ManaRegenTimerHandle);
	bIsRegeneratingMana = false;
}

void UPlayerStatsComponent::DeactivateStaminaRegeneration()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	bIsRegeneratingStamina = false;
}

void UPlayerStatsComponent::RegenerateHealth()
{
	int32 CurrentHealth = ModifyCurrentHealth(HealthRegenRate);
	if (CurrentHealth >= MaxHealth)
	{
		DeactivateHealthRegeneration();
	}
}

void UPlayerStatsComponent::RegenerateMana()
{
	int32 CurrentMana = ModifyCurrentMana(ManaRegenRate);
	if (CurrentMana >= MaxMana)
	{
		DeactivateManaRegeneration();
	}
}

void UPlayerStatsComponent::RegenerateStamina()
{
	int32 CurrentStamina = ModifyCurrentStamina(StaminaRegenRate);
	if (CurrentStamina >= MaxStamina)
	{
		DeactivateStaminaRegeneration();
	}
}
