// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AIStatsComponent.h"
#include "AICharacterBase.h"
#include "FloatingHealthBarWidget.h"
#include "EODWidgetComponent.h"

#include "UnrealNetwork.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

UAIStatsComponent::UAIStatsComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	OwningAIChar = Cast<AAICharacterBase>(GetOwner());
}

void UAIStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAIStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	DOREPLIFETIME(UAIStatsComponent, MovementSpeedModifier);
	DOREPLIFETIME(UAIStatsComponent, ActiveTimeDilation);
	DOREPLIFETIME(UAIStatsComponent, SpellCastingSpeedModifier);
	
	DOREPLIFETIME(UAIStatsComponent, Darkness);
	*/

}

void UAIStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

/*
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
*/
