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

	virtual void ModifyBaseHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBaseHealth, );

	virtual void ModifyMaxHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMaxHealth, );

	virtual void ModifyCurrentHealth(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCurrentHealth, );

	virtual bool IsLowOnHealth() const PURE_VIRTUAL(UStatsComponentBase::IsLowOnHealth, return false; );

	virtual int32 GetBaseMana() const PURE_VIRTUAL(UStatsComponentBase::GetBaseMana, return 0; );

	virtual int32 GetMaxMana() const PURE_VIRTUAL(UStatsComponentBase::GetMaxMana, return 0; );

	virtual int32 GetCurrentMana() const PURE_VIRTUAL(UStatsComponentBase::GetCurrentMana, return 0; );
	
	virtual void ModifyBaseMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBaseMana, );

	virtual void ModifyMaxMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMaxMana, );

	virtual void ModifyCurrentMana(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCurrentMana, );
	
	virtual int32 GetBaseStamina() const PURE_VIRTUAL(UStatsComponentBase::GetBaseStamina, return 0; );

	virtual int32 GetMaxStamina() const PURE_VIRTUAL(UStatsComponentBase::GetMaxStamina, return 0; );

	virtual int32 GetCurrentStamina() const PURE_VIRTUAL(UStatsComponentBase::GetCurrentStamina, return 0; );
	
	virtual void ModifyBaseStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyBaseStamina, );

	virtual void ModifyMaxStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMaxStamina, );

	virtual void ModifyCurrentStamina(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCurrentStamina, );

	virtual int32 GetHealthRegenRate() const PURE_VIRTUAL(UStatsComponentBase::GetHealthRegenRate, return 0; );

	virtual int32 GetManaRegenRate() const PURE_VIRTUAL(UStatsComponentBase::GetManaRegenRate, return 0; );

	virtual int32 GetStaminaRegenRate() const PURE_VIRTUAL(UStatsComponentBase::GetStaminaRegenRate, return 0; );

	virtual int32 GetPhysicalAttack() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalAttack, return 0; );

	virtual int32 GetMagickAttack() const PURE_VIRTUAL(UStatsComponentBase::GetMagickAttack, return 0; );

	virtual int32 GetPhysicalResistance() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalResistance, return 0; );

	virtual int32 GetMagickResistance() const PURE_VIRTUAL(UStatsComponentBase::GetMagickResistance, return 0; );

	virtual void ModifyPhysicalResistance(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyPhysicalResistance, );

	virtual void ModifyMagickalResistance(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyMagickalResistance, );

	virtual float GetPhysicalCritRate() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalCritRate, return 0.f; );

	virtual float GetMagickCritRate() const PURE_VIRTUAL(UStatsComponentBase::GetMagickCritRate, return 0.f; );

	virtual float GetPhysicalCritBonus() const PURE_VIRTUAL(UStatsComponentBase::GetPhysicalCritBonus, return 0.f; );

	virtual float GetMagickCritBonus() const PURE_VIRTUAL(UStatsComponentBase::GetMagickCritBonus, return 0.f; );

	virtual int32 GetElementalFireResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalFireResistance, return 0; );
	
	virtual int32 GetElementalIceResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalIceResistance, return 0; );

	virtual int32 ModifyElementalIceResistance(int32 Value) PURE_VIRTUAL(UStatsComponentBase::ModifyElementalIceResistance, return 0; );
	
	virtual int32 GetElementalLightningResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalLightningResistance, return 0; );
	
	virtual int32 GetElementalPoisonResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalPoisonResistance, return 0; );
	
	virtual int32 GetElementalHolyResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalHolyResistance, return 0; );
	
	virtual int32 GetElementalDarkResistance() const PURE_VIRTUAL(UStatsComponentBase::GetElementalDarkResistance, return 0; );
	
	virtual int32 GetElementalFireDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalFireDamage, return 0; );
	
	virtual int32 GetElementalIceDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalIceDamage, return 0; );
	
	virtual int32 GetElementalLightningDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalLightningDamage, return 0; );
	
	virtual int32 GetElementalPoisonDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalPoisonDamage, return 0; );
	
	virtual int32 GetElementalHolyDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalHolyDamage, return 0; );
	
	virtual int32 GetElementalDarkDamage() const PURE_VIRTUAL(UStatsComponentBase::GetElementalDarkDamage, return 0; );

	virtual int32 GetBleedResistance() const PURE_VIRTUAL(UStatsComponentBase::GetBleedResistance, return 0; );

	virtual int32 GetCrowdControlResistance() const PURE_VIRTUAL(UStatsComponentBase::GetCrowdControlResistance, return 0; );

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

	virtual void ModifyActiveTimeDilation(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyActiveTimeDilation, );

	virtual float GetSpellCastingSpeedModifier() const PURE_VIRTUAL(UStatsComponentBase::GetSpellCastingSpeedModifier, return 0.f; );

	virtual int32 GetDarkness() const PURE_VIRTUAL(UStatsComponentBase::GetDarkness, return 0; );

protected:

	//~ @note following booleans are used to initialize timers for derived stats components

	UPROPERTY(EditDefaultsOnly, Category = Conditionals)
	uint32 bHasHealthRegenration : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = Conditionals)
	uint32 bHasManaRegenration : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = Conditionals)
	uint32 bHasStaminaRegenration : 1;

		
	
};
