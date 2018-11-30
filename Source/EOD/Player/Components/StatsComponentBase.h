// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CombatLibrary.h"
#include "Components/ActorComponent.h"
#include "StatsComponentBase.generated.h"

/**
 * An abstract base class that lays out the expected behavior of stats component to manage character stats.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API UStatsComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	

	/** Sets default values for this component's properties */
	UStatsComponentBase(const FObjectInitializer& ObjectInitializer);
	
	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual int32 GetBaseHealth() const PURE_VIRTUAL(UStatsComponentBase::GetBaseHealth, return 0; );
	
	virtual int32 GetMaxHealth() const PURE_VIRTUAL(UStatsComponentBase::GetMaxHealth, return 0; );

	virtual int32 GetCurrentHealth() const PURE_VIRTUAL(UStatsComponentBase::GetCurrentHealth, return 0; );

	virtual int32 ModifyBaseHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBaseHealth, return 0; );

	virtual int32 ModifyMaxHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMaxHealth, return 0; );

	virtual int32 ModifyCurrentHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCurrentHealth, return 0; );

	virtual void SetBaseHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetBaseHealth, );

	virtual void SetMaxHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetMaxHealth, );

	virtual void SetCurrentHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetCurrentHealth, );

	virtual bool IsLowOnHealth() const PURE_VIRTUAL(UStatsComponentBase::IsLowOnHealth, return false; );
	
	virtual int32 GetBaseMana() const PURE_VIRTUAL(UStatsComponentBase::GetBaseMana, return 0; );

	virtual int32 GetMaxMana() const PURE_VIRTUAL(UStatsComponentBase::GetMaxMana, return 0; );

	virtual int32 GetCurrentMana() const PURE_VIRTUAL(UStatsComponentBase::GetCurrentMana, return 0; );
	
	virtual int32 ModifyBaseMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBaseMana, return 0; );

	virtual int32 ModifyMaxMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMaxMana, return 0; );

	virtual int32 ModifyCurrentMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCurrentMana, return 0; );

	virtual void SetBaseMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetBaseMana, );

	virtual void SetMaxMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetMaxMana, );

	virtual void SetCurrentMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetCurrentMana, );
	
	virtual int32 GetBaseStamina() const PURE_VIRTUAL(UStatsComponentBase::GetBaseStamina, return 0; );

	virtual int32 GetMaxStamina() const PURE_VIRTUAL(UStatsComponentBase::GetMaxStamina, return 0; );

	virtual int32 GetCurrentStamina() const PURE_VIRTUAL(UStatsComponentBase::GetCurrentStamina, return 0; );
	
	virtual int32 ModifyBaseStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBaseStamina, return 0; );

	virtual int32 ModifyMaxStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMaxStamina, return 0; );

	virtual int32 ModifyCurrentStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCurrentStamina, return 0; );

	virtual void SetBaseStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetBaseStamina, );

	virtual void SetMaxStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetMaxStamina, );

	virtual void SetCurrentStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetCurrentStamina, );

	virtual int32 GetHealthRegenRate() const PURE_VIRTUAL(UStatsComponentBase::GetHealthRegenRate, return 0; );

	virtual int32 ModifyHealthRegenRate(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyHealthRegenRate, return 0; );

	virtual void SetHealthRegenRate(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetHealthRegenRate, );

	virtual int32 GetManaRegenRate() const PURE_VIRTUAL(UStatsComponentBase::GetManaRegenRate, return 0; );

	virtual int32 ModifyManaRegenRate(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyManaRegenRate, return 0; );

	virtual void SetManaRegenRate(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetManaRegenRate, );

	virtual int32 GetStaminaRegenRate() const PURE_VIRTUAL(UStatsComponentBase::GetStaminaRegenRate, return 0; );

	virtual int32 ModifyStaminaRegenRate(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyStaminaRegenRate, return 0; );

	virtual void SetStaminaRegenRate(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetStaminaRegenRate, );

	virtual int32 GetPhysicalAttack() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalAttack, return 0; );

	virtual int32 GetMagickAttack() const PURE_VIRTUAL(UStatsComponentBase::GetMagickAttack, return 0; );

	virtual int32 ModifyPhysicalAttack(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyPhysicalAttack, return 0; );

	virtual int32 ModifyMagickAttack(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMagickAttack, return 0; );

	virtual void SetPhysicalAttack(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetPhysicalAttack, );

	virtual void SetMagickAttack(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetMagickAttack, );

	virtual int32 GetPhysicalResistance() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalResistance, return 0; );

	virtual int32 GetMagickResistance() const PURE_VIRTUAL(UStatsComponentBase::GetMagickResistance, return 0; );

	virtual int32 ModifyPhysicalResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyPhysicalResistance, return 0; );

	virtual int32 ModifyMagickResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMagickResistance, return 0; );

	virtual void SetPhysicalResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetPhysicalResistance, );

	virtual void SetMagickResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetMagickResistance, );

	virtual float GetPhysicalCritRate() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalCritRate, return 0.f; );

	virtual float GetMagickCritRate() const PURE_VIRTUAL(UStatsComponentBase::GetMagickCritRate, return 0.f; );
	
	virtual float ModifyPhysicalCritRate(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyPhysicalCritRate, return 0.f; );

	virtual float ModifyMagickCritRate(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMagickCritRate, return 0.f; );

	virtual void SetPhysicalCritRate(float Value) PURE_VIRTUAL(UStatsComponentBase::SetPhysicalCritRate, );

	virtual void SetMagickCritRate(float Value) PURE_VIRTUAL(UStatsComponentBase::SetMagickCritRate, );

	virtual int32 GetPhysicalCritBonus() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalCritBonus, return 0; );

	virtual int32 GetMagickCritBonus() const PURE_VIRTUAL(UStatsComponentBase::GetMagickCritBonus, return 0; );
	
	virtual int32 ModifyPhysicalCritBonus(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyPhysicalCritBonus, return 0; );

	virtual int32 ModifyMagickCritBonus(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMagickCritBonus, return 0; );

	virtual void SetPhysicalCritBonus(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetPhysicalCritBonus, );

	virtual void SetMagickCritBonus(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetMagickCritBonus, );

	virtual int32 GetElementalFireResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalFireResistance, return 0; );
	
	virtual int32 GetElementalIceResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalIceResistance, return 0; );
	
	virtual int32 GetElementalLightningResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalLightningResistance, return 0; );
	
	virtual int32 GetElementalPoisonResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalPoisonResistance, return 0; );
	
	virtual int32 GetElementalHolyResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalHolyResistance, return 0; );
	
	virtual int32 GetElementalDarkResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalDarkResistance, return 0; );
	
	virtual int32 ModifyElementalFireResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalFireResistance, return 0; );
	
	virtual int32 ModifyElementalIceResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalIceResistance, return 0; );
	
	virtual int32 ModifyElementalLightningResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalLightningResistance, return 0; );
	
	virtual int32 ModifyElementalPoisonResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalPoisonResistance, return 0; );
	
	virtual int32 ModifyElementalHolyResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalHolyResistance, return 0; );
	
	virtual int32 ModifyElementalDarkResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalDarkResistance, return 0; );
	
	virtual void SetElementalFireResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalFireResistance, );
	
	virtual void SetElementalIceResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalIceResistance, );
	
	virtual void SetElementalLightningResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalLightningResistance, );
	
	virtual void SetElementalPoisonResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalPoisonResistance, );
	
	virtual void SetElementalHolyResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalHolyResistance, );
	
	virtual void SetElementalDarkResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalDarkResistance, );
	
	virtual int32 GetElementalFireDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalFireDamage, return 0; );
	
	virtual int32 GetElementalIceDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalIceDamage, return 0; );
	
	virtual int32 GetElementalLightningDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalLightningDamage, return 0; );
	
	virtual int32 GetElementalPoisonDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalPoisonDamage, return 0; );
	
	virtual int32 GetElementalHolyDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalHolyDamage, return 0; );
	
	virtual int32 GetElementalDarkDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalDarkDamage, return 0; );
	
	virtual int32 ModifyElementalFireDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalFireDamage, return 0; );
	
	virtual int32 ModifyElementalIceDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalIceDamage, return 0; );
	
	virtual int32 ModifyElementalLightningDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalLightningDamage, return 0; );
	
	virtual int32 ModifyElementalPoisonDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalPoisonDamage, return 0; );
	
	virtual int32 ModifyElementalHolyDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalHolyDamage, return 0; );
	
	virtual int32 ModifyElementalDarkDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalDarkDamage, return 0; );
	
	virtual void SetElementalFireDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalFireDamage, );
	
	virtual void SetElementalIceDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalIceDamage, );
	
	virtual void SetElementalLightningDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalLightningDamage, );
	
	virtual void SetElementalPoisonDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalPoisonDamage, );
	
	virtual void SetElementalHolyDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalHolyDamage, );
	
	virtual void SetElementalDarkDamage(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetElementalDarkDamage, );

	virtual int32 GetBleedResistance() const PURE_VIRTUAL(UStatsComponentBase::GetBleedResistance, return 0; );

	virtual int32 GetCrowdControlResistance() const PURE_VIRTUAL(UStatsComponentBase::GetCrowdControlResistance, return 0; );
	
	virtual int32 ModifyBleedResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBleedResistance, return 0; );
	
	virtual int32 ModifyCrowdControlResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCrowdControlResistance, return 0; );
	
	virtual void SetBleedResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetBleedResistance, );
	
	virtual void SetCrowdControlResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetCrowdControlResistance, );

	virtual void AddCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) PURE_VIRTUAL(UStatsComponentBase::AddCrowdControlImmunity, );

	virtual void AddCrowdControlImmunities(uint8 CrowdControlImmunities) PURE_VIRTUAL(UStatsComponentBase::AddCrowdControlImmunities, );

	virtual void RemoveCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) PURE_VIRTUAL(UStatsComponentBase::RemoveCrowdControlImmunity, );

	virtual void RemoveCrowdControlImmunities(uint8 CrowdControlImmunities) PURE_VIRTUAL(UStatsComponentBase::RemoveCrowdControlImmunity, );

	virtual void RemoveAllCrowdControlImmunities() PURE_VIRTUAL(UStatsComponentBase::RemoveAllCrowdControlImmunity, );

	virtual bool HasCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) const PURE_VIRTUAL(UStatsComponentBase::HasCrowdControlImmunity, return false; );

	virtual uint8 GetCrowdControlImmunities() const PURE_VIRTUAL(UStatsComponentBase::GetCrowdControlImmunities, return 0; );

	virtual float GetCooldownModifier() const PURE_VIRTUAL(UStatsComponentBase::GetCooldownModifier, return 0.f; );

	virtual float GetExpModifier() const PURE_VIRTUAL(UStatsComponentBase::GetExpModifier, return 0.f; );

	virtual float GetDropRateModifier() const PURE_VIRTUAL(UStatsComponentBase::GetDropRateModifier, return 0.f; );

	virtual float GetStaminaConsumptionModifier() const PURE_VIRTUAL(UStatsComponentBase::GetStaminaConsumptionModifier, return 0.f; );

	virtual float GetMovementSpeedModifier() const PURE_VIRTUAL(UStatsComponentBase::GetMovementSpeedModifier, return 0.f; );

	virtual float GetActiveTimeDilation() const PURE_VIRTUAL(UStatsComponentBase::GetActiveTimeDilation, return 0.f; );
	
	virtual float GetSpellCastingSpeedModifier() const PURE_VIRTUAL(UStatsComponentBase::GetSpellCastingSpeedModifier, return 0.f; );
	
	virtual float ModifyCooldownModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCooldownModifier, return 0.f; );
	
	virtual float ModifyExpModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyExpModifier, return 0.f; );

	virtual float ModifyDropRateModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyDropRateModifier, return 0.f; );
	
	virtual float ModifyStaminaConsumptionModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyStaminaConsumptionModifier, return 0.f; );
	
	virtual float ModifyMovementSpeedModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMovementSpeedModifier, return 0.f; );
	
	virtual float ModifyActiveTimeDilation(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyActiveTimeDilation, return 0.f; );
	
	virtual float ModifySpellCastingSpeedModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifySpellCastingSpeedModifier, return 0.f; );

	virtual void SetCooldownModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetCooldownModifier, );
	
	virtual void SetExpModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetExpModifier, );
	
	virtual void SetDropRateModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetDropRateModifier, );
	
	virtual void SetStaminaConsumptionModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetStaminaConsumptionModifier, );
	
	virtual void SetMovementSpeedModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetMovementSpeedModifier, );
	
	virtual void SetActiveTimeDilation(float Value) PURE_VIRTUAL(UStatsComponentBase::SetActiveTimeDilation, );
	
	virtual void SetSpellCastingSpeedModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetSpellCastingSpeedModifier, );

	virtual int32 GetDarkness() const PURE_VIRTUAL(UStatsComponentBase::GetDarkness, return 0; );

	virtual int32 ModifyDarkness(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyDarkness, return 0; );

	virtual void SetDarkness(int32 Value) PURE_VIRTUAL(UStatsComponentBase::SetDarkness, );

	virtual float GetPhysicalDamageReductionOnBlock() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalDamageReductionOnBlock, return 0.f; );

	virtual float GetMagickDamageReductionOnBlock() const PURE_VIRTUAL(UStatsComponentBase::GetMagickDamageReductionOnBlock, return 0.f; );

	virtual float ModifyPhysicalDamageReductionOnBlock(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyPhysicalDamageReductionOnBlock, return 0.f; );

	virtual float ModifyMagickDamageReductionOnBlock(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMagickDamageReductionOnBlock, return 0.f; );

	virtual void SetPhysicalDamageReductionOnBlock(float Value) PURE_VIRTUAL(UStatsComponentBase::SetPhysicalDamageReductionOnBlock, );

	virtual void SetMagickDamageReductionOnBlock(float Value) PURE_VIRTUAL(UStatsComponentBase::SetMagickDamageReductionOnBlock, );

	// virtual void InitializeComponentWidget() PURE_VIRTUAL(UStatsComponentBase::InitializeComponentWidget, );

	virtual void AddCrowdControlImmunitiesFromSkill(uint8 CCImmunities) PURE_VIRTUAL(UStatsComponentBase::AddCrowdControlImmunitiesFromSkill, );

	virtual void RemoveCrowdControlImmunitiesFromSkil() PURE_VIRTUAL(UStatsComponentBase::RemoveCrowdControlImmunitiesFromSkil, );

	virtual uint8 GetCrowdControlImmunitiesFromSkill() const PURE_VIRTUAL(UStatsComponentBase::GetCrowdControlImmunitiesFromSkill, return 0; );

protected:

	//~ @note following booleans are used to initialize timers for derived stats components

	UPROPERTY(EditDefaultsOnly, Category = Regeneration)
	uint32 bHasHealthRegenration : 1;

	UPROPERTY(EditDefaultsOnly, Category = Regeneration)
	uint32 bHasManaRegenration : 1;

	UPROPERTY(EditDefaultsOnly, Category = Regeneration)
	uint32 bHasStaminaRegenration : 1;

private:



		
	
};
