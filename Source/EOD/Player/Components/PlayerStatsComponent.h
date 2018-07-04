// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/Components/BaseStatsComponent.h"
#include "PlayerStatsComponent.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UPlayerStatsComponent : public UBaseStatsComponent
{
	GENERATED_BODY()
	
public:

	UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer);

	// Property replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual int32 GetBaseHealth() const override;
	
	virtual int32 GetMaxHealth() const override;

	virtual int32 GetCurrentHealth() const override;

	virtual void ModifyBaseHealth(int32 Value) override;

	virtual void ModifyMaxHealth(int32 Value) override;

	virtual void ModifyCurrentHealth(int32 Value) override;

	virtual bool IsLowOnHealth() const override;

	virtual int32 GetBaseMana() const override;

	virtual int32 GetMaxMana() const override;

	virtual int32 GetCurrentMana() const override;
	
	virtual void ModifyBaseMana(int32 Value) override;

	virtual void ModifyMaxMana(int32 Value) override;

	virtual void ModifyCurrentMana(int32 Value) override;
	
	virtual int32 GetBaseStamina() const override;

	virtual int32 GetMaxStamina() const override;

	virtual int32 GetCurrentStamina() const override;
	
	virtual void ModifyBaseStamina(int32 Value) override;

	virtual void ModifyMaxStamina(int32 Value) override;

	virtual void ModifyCurrentStamina(int32 Value) override;

	virtual int32 GetHealthRegenRate() const override;

	virtual int32 GetManaRegenRate() const override;

	virtual int32 GetStaminaRegenRate() const override;

	virtual int32 GetPhysicalAttack() const override;

	virtual int32 GetMagickAttack() const override;

	virtual int32 GetPhysicalResistance() const override;

	virtual int32 GetMagickResistance() const override;

	virtual float GetPhysicalCritRate() const override;

	virtual float GetMagickCritRate() const override;

	virtual float GetPhysicalCritBonus() const override;

	virtual float GetMagickCritBonus() const override;

	virtual int32 GetElementalFireResistance() const override;
	
	virtual int32 GetElementalIceResistance() const override;
	
	virtual int32 GetElementalLightningResistance() const override;
	
	virtual int32 GetElementalPoisonResistance() const override;
	
	virtual int32 GetElementalHolyResistance() const override;
	
	virtual int32 GetElementalDarkResistance() const override;
	
	virtual int32 GetElementalFireDamage() const override;
	
	virtual int32 GetElementalIceDamage() const override;
	
	virtual int32 GetElementalLightningDamage() const override;
	
	virtual int32 GetElementalPoisonDamage() const override;
	
	virtual int32 GetElementalHolyDamage() const override;
	
	virtual int32 GetElementalDarkDamage() const override;

	virtual int32 GetBleedResistance() const override;

	virtual int32 GetCrowdControlResistance() const override;

	// @todo crowd control immunities

	virtual float GetCooldownModifier() const override;

	virtual float GetExpModifier() const override;

	virtual float GetDropRateModifier() const override;

	virtual float GetStaminaConsumptionModifer() const override;

	virtual float GetMovementSpeedModifier() const override;

	virtual float GetAnimationSpeedModifier() const override;

	virtual float GetSpellCastingSpeedModifier() const override;

	virtual int32 GetDarkness() const override;

private:
	
	// @note All changes to variables similar to MaxHealth, CurrentHealth, etc. will occur ONLY on server and will automatically get replicated. No RPC needed.

	/**
	 * Current player level
	 * @note Player level will change rarely and therefore does not need to be set to replicate
	 * @todo Multicast for replicating level changes
	*/
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 Level;

	/**
	 * Maximum health of character without any status effects
	 * Rarely changes; not replicated
	 * @todo An RPC (most probably a client RPC) to replicate changes in BaseHealth to owning client
	*/
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseHealth;

	/**
	 * Current maximum health of character - with or without any status effects
	 * Changes often to status effects; replicated
	*/
	UPROPERTY(Replicated)
	int32 MaxHealth;

	/**
	 * Current health of character
	 * Changes often; replicated
	*/
	/** Current health of character */
	UPROPERTY(Replicated)
	int32 CurrentHealth;
	
	/** Maximum mana of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseMana;

	UPROPERTY(Replicated)
	int32 MaxMana;

	UPROPERTY(Replicated)
	int32 CurrentMana;
	
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseStamina;

	UPROPERTY(Replicated)
	int32 MaxStamina;

	UPROPERTY(Replicated)
	int32 CurrentStamina;

	/**
	 * @note
	 * Regeneration rate could change a lot due to status effects or change in combat status
	 * But it is not necessary to replicate this variable to all clients except the owning client
	 * The health regeneration will automatically get replicated through CurrentHealth
	*/

	//~ @todo If regeneration rates does not need to be displayed in character UI
	// then they can simply be set to not replicate

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 HealthRegenRate;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 ManaRegenRate;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 StaminaRegenRate;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	int32 PhysicalAttack;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats)
	int32 PhysicalResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	int32 MagickAttack;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats)
	int32 MagickResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	float PhysicalCritRate;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	float MagickCritRate;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	int32 PhysicalCritBonus;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats)
	int32 MagickCritBonus;

	//~ @todo replication conditions for ElementalDamage and ElementalResistance
	/** 
	 * @note: variables like ElementalResistance may not need to be replicated.
	 * The server can simply do calculations with it's own copy of ElementalResistance
	 * and pass on the result to clients.
	 */

	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalFireDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalIceDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalPoisonDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalLightningDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalHolyDamage;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalDarkDamage;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalFireResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalIceResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalPoisonResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalLightningResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalHolyResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalDarkResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 BleedResistance;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 CrowdControlResistance;
	
	/**
	 * @note
	 * CooldownModifier, ExpModifier, DropRateModifier, and StaminaConsumptionModifier
	 * will be replicated to owner only. They are irrelevant to other clients.
	*/

	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float CooldownModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float ExpModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float DropRateModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float StaminaConsumptionModifier;

	/**
	 * @note
	 * MovementSpeedModifier, AnimationSpeedModifier, SpellCastingSpeedModifier, and
	 * Darkness will be replicated to all clients
	*/

	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float MovementSpeedModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float AnimationSpeedModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float SpellCastingSpeedModifier;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats)
	int32 Darkness;
	
};
