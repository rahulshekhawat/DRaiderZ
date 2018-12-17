// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/AI/Components/AIStatsComponent.h"
#include "EOD/Characters/AICharacterBase.h"
#include "UI/FloatingHealthBarWidget.h"
#include "Player/Components/EODWidgetComponent.h"

#include "UnrealNetwork.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

UAIStatsComponent::UAIStatsComponent(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	OwningAIChar = Cast<AAICharacterBase>(GetOwner());
}

void UAIStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	/*
	SetMaxHealth(BaseHealth);
	SetCurrentHealth(BaseHealth);

	SetMaxMana(BaseMana);
	SetCurrentMana(BaseMana);
	*/

}

void UAIStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	DOREPLIFETIME(UAIStatsComponent, MaxHealth);
	DOREPLIFETIME(UAIStatsComponent, CurrentHealth);
	DOREPLIFETIME(UAIStatsComponent, MaxMana);
	DOREPLIFETIME(UAIStatsComponent, CurrentMana);
	*/

	/*
	DOREPLIFETIME(UAIStatsComponent, MovementSpeedModifier);
	DOREPLIFETIME(UAIStatsComponent, ActiveTimeDilation);
	DOREPLIFETIME(UAIStatsComponent, SpellCastingSpeedModifier);
	
	DOREPLIFETIME(UAIStatsComponent, Darkness);
	*/

}

/*
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
	int32 Result = CurrentHealth + Value;
	Result = Result <= 0 ? 0 : Result;
	SetCurrentHealth(Result);
	return Result;
}

void UAIStatsComponent::SetBaseHealth(int32 Value)
{
}

void UAIStatsComponent::SetMaxHealth(int32 Value)
{
	MaxHealth = Value;
}

void UAIStatsComponent::SetCurrentHealth(int32 Value)
{
	CurrentHealth = Value;

	UFloatingHealthBarWidget* HealthWidget = Cast<UFloatingHealthBarWidget>(OwningAIChar->GetHealthWidgetComp()->GetUserWidgetObject());
	if (HealthWidget && MaxHealth != 0)
	{
		float Percent = (float)CurrentHealth / (float)MaxHealth;
		HealthWidget->HealthBar->SetPercent(Percent);
	}
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

void UAIStatsComponent::SetBaseMana(int32 Value)
{
}

void UAIStatsComponent::SetMaxMana(int32 Value)
{
}

void UAIStatsComponent::SetCurrentMana(int32 Value)
{
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

void UAIStatsComponent::SetBaseStamina(int32 Value)
{
}

void UAIStatsComponent::SetMaxStamina(int32 Value)
{
}

void UAIStatsComponent::SetCurrentStamina(int32 Value)
{
}
*/

int32 UAIStatsComponent::GetHealthRegenRate() const
{
	return HealthRegenRate;
}

int32 UAIStatsComponent::ModifyHealthRegenRate(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetHealthRegenRate(int32 Value)
{
}

int32 UAIStatsComponent::GetManaRegenRate() const
{
	return ManaRegenRate;
}

int32 UAIStatsComponent::ModifyManaRegenRate(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetManaRegenRate(int32 Value)
{
}

int32 UAIStatsComponent::GetStaminaRegenRate() const
{
	// AI stats component does not implement StaminaRegenRate
	return 0;
}

int32 UAIStatsComponent::ModifyStaminaRegenRate(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetStaminaRegenRate(int32 Value)
{
}

int32 UAIStatsComponent::GetPhysicalAttack() const
{
	return PhysicalAttack;
}

int32 UAIStatsComponent::GetMagickAttack() const
{
	return MagickAttack;
}

int32 UAIStatsComponent::ModifyPhysicalAttack(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyMagickAttack(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetPhysicalAttack(int32 Value)
{
}

void UAIStatsComponent::SetMagickAttack(int32 Value)
{
}

int32 UAIStatsComponent::GetPhysicalResistance() const
{
	return PhysicalResistance;
}

int32 UAIStatsComponent::GetMagickResistance() const
{
	return MagickResistance;
}

int32 UAIStatsComponent::ModifyPhysicalResistance(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyMagickResistance(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetPhysicalResistance(int32 Value)
{
}

void UAIStatsComponent::SetMagickResistance(int32 Value)
{
}

float UAIStatsComponent::GetPhysicalCritRate() const
{
	return PhysicalCritRate;
}

float UAIStatsComponent::GetMagickCritRate() const
{
	return MagickCritRate;
}

float UAIStatsComponent::ModifyPhysicalCritRate(float Value)
{
	return 0.0f;
}

float UAIStatsComponent::ModifyMagickCritRate(float Value)
{
	return 0.0f;
}

void UAIStatsComponent::SetPhysicalCritRate(float Value)
{
}

void UAIStatsComponent::SetMagickCritRate(float Value)
{
}

int32 UAIStatsComponent::GetPhysicalCritBonus() const
{
	return PhysicalCritBonus;
}

int32 UAIStatsComponent::GetMagickCritBonus() const
{
	return MagickCritBonus;
}

int32 UAIStatsComponent::ModifyPhysicalCritBonus(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyMagickCritBonus(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetPhysicalCritBonus(int32 Value)
{
}

void UAIStatsComponent::SetMagickCritBonus(int32 Value)
{
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

int32 UAIStatsComponent::ModifyElementalLightningResistance(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalPoisonResistance(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalHolyResistance(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalDarkResistance(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetElementalFireResistance(int32 Value)
{
}

void UAIStatsComponent::SetElementalIceResistance(int32 Value)
{
}

void UAIStatsComponent::SetElementalLightningResistance(int32 Value)
{
}

void UAIStatsComponent::SetElementalPoisonResistance(int32 Value)
{
}

void UAIStatsComponent::SetElementalHolyResistance(int32 Value)
{
}

void UAIStatsComponent::SetElementalDarkResistance(int32 Value)
{
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

int32 UAIStatsComponent::ModifyElementalFireResistance(int32 Value)
{
	return int32();
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

int32 UAIStatsComponent::ModifyElementalFireDamage(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalIceDamage(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalLightningDamage(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalPoisonDamage(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyElementalHolyDamage(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::GetBleedResistance() const
{
	return BleedResistance;
}

int32 UAIStatsComponent::GetCrowdControlResistance() const
{
	return CrowdControlResistance;
}

int32 UAIStatsComponent::ModifyBleedResistance(int32 Value)
{
	return int32();
}

int32 UAIStatsComponent::ModifyCrowdControlResistance(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetBleedResistance(int32 Value)
{
}

void UAIStatsComponent::SetCrowdControlResistance(int32 Value)
{
}

void UAIStatsComponent::AddCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect)
{
}

void UAIStatsComponent::AddCrowdControlImmunities(uint8 CCImmunities)
{
}

void UAIStatsComponent::RemoveCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect)
{
}

void UAIStatsComponent::RemoveCrowdControlImmunities(uint8 CCImmunities)
{
}

void UAIStatsComponent::RemoveAllCrowdControlImmunities()
{
}

bool UAIStatsComponent::HasCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) const
{
	return false;
}

uint8 UAIStatsComponent::GetCrowdControlImmunities() const
{
	return uint8();
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

float UAIStatsComponent::ModifySpellCastingSpeedModifier(float Value)
{
	return 0.0f;
}

void UAIStatsComponent::SetCooldownModifier(float Value)
{
}

void UAIStatsComponent::SetExpModifier(float Value)
{
}

void UAIStatsComponent::SetDropRateModifier(float Value)
{
}

void UAIStatsComponent::SetStaminaConsumptionModifier(float Value)
{
}

void UAIStatsComponent::SetMovementSpeedModifier(float Value)
{
}

void UAIStatsComponent::SetActiveTimeDilation(float Value)
{
}

void UAIStatsComponent::SetSpellCastingSpeedModifier(float Value)
{
}

float UAIStatsComponent::GetSpellCastingSpeedModifier() const
{
	return SpellCastingSpeedModifier;
}

float UAIStatsComponent::ModifyCooldownModifier(float Value)
{
	return 0.0f;
}

float UAIStatsComponent::ModifyExpModifier(float Value)
{
	return 0.0f;
}

float UAIStatsComponent::ModifyDropRateModifier(float Value)
{
	return 0.0f;
}

float UAIStatsComponent::ModifyStaminaConsumptionModifier(float Value)
{
	return 0.0f;
}

float UAIStatsComponent::ModifyMovementSpeedModifier(float Value)
{
	return 0.0f;
}

int32 UAIStatsComponent::GetDarkness() const
{
	return Darkness;
}

int32 UAIStatsComponent::ModifyDarkness(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetDarkness(int32 Value)
{
}

float UAIStatsComponent::GetPhysicalDamageReductionOnBlock() const
{
	return 0.0f;
}

float UAIStatsComponent::GetMagickDamageReductionOnBlock() const
{
	return 0.0f;
}

float UAIStatsComponent::ModifyPhysicalDamageReductionOnBlock(float Value)
{
	return 0.0f;
}

float UAIStatsComponent::ModifyMagickDamageReductionOnBlock(float Value)
{
	return 0.0f;
}

void UAIStatsComponent::SetPhysicalDamageReductionOnBlock(float Value)
{
}

void UAIStatsComponent::SetMagickDamageReductionOnBlock(float Value)
{
}

int32 UAIStatsComponent::ModifyElementalDarkDamage(int32 Value)
{
	return int32();
}

void UAIStatsComponent::SetElementalFireDamage(int32 Value)
{
}

void UAIStatsComponent::SetElementalIceDamage(int32 Value)
{
}

void UAIStatsComponent::SetElementalLightningDamage(int32 Value)
{
}

void UAIStatsComponent::SetElementalPoisonDamage(int32 Value)
{
}

void UAIStatsComponent::SetElementalHolyDamage(int32 Value)
{
}

void UAIStatsComponent::SetElementalDarkDamage(int32 Value)
{
}
