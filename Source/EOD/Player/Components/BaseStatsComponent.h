// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStatsComponent.generated.h"

/**
 * An abstract base class that lays out the expected behavior of stats component to manage character stats.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EOD_API UBaseStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Sets default values for this component's properties */
	UBaseStatsComponent(const FObjectInitializer& ObjectInitializer);
	
	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	virtual int32 GetBaseHealth() const PURE_VIRTUAL(UBaseStatsComponent::GetBaseHealth, return 0; );
	
	virtual int32 GetMaxHealth() const PURE_VIRTUAL(UBaseStatsComponent::GetMaxHealth, return 0; );

	virtual int32 GetCurrentHealth() const PURE_VIRTUAL(UBaseStatsComponent::GetCurrentHealth, return 0; );

	virtual void ModifyBaseHealth(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyBaseHealth, );

	virtual void ModifyMaxHealth(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyMaxHealth, );

	virtual void ModifyCurrentHealth(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyCurrentHealth, );

	virtual bool IsLowOnHealth() const PURE_VIRTUAL(UBaseStatsComponent::IsLowOnHealth, return false; );

	virtual int32 GetBaseMana() const PURE_VIRTUAL(UBaseStatsComponent::GetBaseMana, return 0; );

	virtual int32 GetMaxMana() const PURE_VIRTUAL(UBaseStatsComponent::GetMaxMana, return 0; );

	virtual int32 GetCurrentMana() const PURE_VIRTUAL(UBaseStatsComponent::GetCurrentMana, return 0; );
	
	virtual void ModifyBaseMana(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyBaseMana, );

	virtual void ModifyMaxMana(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyMaxMana, );

	virtual void ModifyCurrentMana(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyCurrentMana, );
	
	virtual int32 GetBaseStamina() const PURE_VIRTUAL(UBaseStatsComponent::GetBaseStamina, return 0; );

	virtual int32 GetMaxStamina() const PURE_VIRTUAL(UBaseStatsComponent::GetMaxStamina, return 0; );

	virtual int32 GetCurrentStamina() const PURE_VIRTUAL(UBaseStatsComponent::GetCurrentStamina, return 0; );
	
	virtual void ModifyBaseStamina(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyBaseStamina, );

	virtual void ModifyMaxStamina(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyMaxStamina, );

	virtual void ModifyCurrentStamina(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyCurrentStamina, );

	virtual int32 GetHealthRegenRate() const PURE_VIRTUAL(UBaseStatsComponent::GetHealthRegenRate, return 0; );

	virtual int32 GetManaRegenRate() const PURE_VIRTUAL(UBaseStatsComponent::GetManaRegenRate, return 0; );

	virtual int32 GetStaminaRegenRate() const PURE_VIRTUAL(UBaseStatsComponent::GetStaminaRegenRate, return 0; );

	virtual int32 GetPhysicalAttack() const PURE_VIRTUAL(UBaseStatsComponent::GetPhysicalAttack, return 0; );

	virtual int32 GetMagickAttack() const PURE_VIRTUAL(UBaseStatsComponent::GetMagickAttack, return 0; );

	virtual int32 GetPhysicalResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetPhysicalResistance, return 0; );

	virtual int32 GetMagickResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetMagickResistance, return 0; );

	virtual float GetPhysicalCritRate() const PURE_VIRTUAL(UBaseStatsComponent::GetPhysicalCritRate, return 0.f; );

	virtual float GetMagickCritRate() const PURE_VIRTUAL(UBaseStatsComponent::GetMagickCritRate, return 0.f; );

	virtual float GetPhysicalCritBonus() const PURE_VIRTUAL(UBaseStatsComponent::GetPhysicalCritBonus, return 0.f; );

	virtual float GetMagickCritBonus() const PURE_VIRTUAL(UBaseStatsComponent::GetMagickCritBonus, return 0.f; );

	virtual int32 GetElementalFireResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalFireResistance, return 0; );
	
	virtual int32 GetElementalIceResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalIceResistance, return 0; );

	virtual int32 ModifyElementalIceResistance(int32 Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyElementalIceResistance, return 0; );
	
	virtual int32 GetElementalLightningResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalLightningResistance, return 0; );
	
	virtual int32 GetElementalPoisonResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalPoisonResistance, return 0; );
	
	virtual int32 GetElementalHolyResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalHolyResistance, return 0; );
	
	virtual int32 GetElementalDarkResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalDarkResistance, return 0; );
	
	virtual int32 GetElementalFireDamage() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalFireDamage, return 0; );
	
	virtual int32 GetElementalIceDamage() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalIceDamage, return 0; );
	
	virtual int32 GetElementalLightningDamage() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalLightningDamage, return 0; );
	
	virtual int32 GetElementalPoisonDamage() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalPoisonDamage, return 0; );
	
	virtual int32 GetElementalHolyDamage() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalHolyDamage, return 0; );
	
	virtual int32 GetElementalDarkDamage() const PURE_VIRTUAL(UBaseStatsComponent::GetElementalDarkDamage, return 0; );

	virtual int32 GetBleedResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetBleedResistance, return 0; );

	virtual int32 GetCrowdControlResistance() const PURE_VIRTUAL(UBaseStatsComponent::GetCrowdControlResistance, return 0; );

	// @todo crowd control immunities

	virtual float GetCooldownModifier() const PURE_VIRTUAL(UBaseStatsComponent::GetCooldownModifier, return 0.f; );

	virtual float GetExpModifier() const PURE_VIRTUAL(UBaseStatsComponent::GetExpModifier, return 0.f; );

	virtual float GetDropRateModifier() const PURE_VIRTUAL(UBaseStatsComponent::GetDropRateModifier, return 0.f; );

	virtual float GetStaminaConsumptionModifier() const PURE_VIRTUAL(UBaseStatsComponent::GetStaminaConsumptionModifier, return 0.f; );

	virtual float GetMovementSpeedModifier() const PURE_VIRTUAL(UBaseStatsComponent::GetMovementSpeedModifier, return 0.f; );

	virtual float GetActiveTimeDilation() const PURE_VIRTUAL(UBaseStatsComponent::GetActiveTimeDilation, return 0.f; );

	virtual void ModifyActiveTimeDilation(float Value) PURE_VIRTUAL(UBaseStatsComponent::ModifyActiveTimeDilation, );

	virtual float GetSpellCastingSpeedModifier() const PURE_VIRTUAL(UBaseStatsComponent::GetSpellCastingSpeedModifier, return 0.f; );

	virtual int32 GetDarkness() const PURE_VIRTUAL(UBaseStatsComponent::GetDarkness, return 0; );

protected:

	//~ @note following booleans are used to initialize timers for derived stats components

	UPROPERTY(EditDefaultsOnly, Category = Conditionals)
	uint32 bHasHealthRegenration : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = Conditionals)
	uint32 bHasManaRegenration : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = Conditionals)
	uint32 bHasStaminaRegenration : 1;

	
};
