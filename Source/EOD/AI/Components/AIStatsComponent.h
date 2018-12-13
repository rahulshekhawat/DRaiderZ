// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Player/Components/StatsComponentBase.h"
#include "AIStatsComponent.generated.h"

class AAICharacterBase;

/**
 * AIStatsComponent is used to manage stats of an AI controlled character
 */
UCLASS()
class EOD_API UAIStatsComponent : public UStatsComponentBase
{
	GENERATED_BODY()
	
public:

	UAIStatsComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	
	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*
	virtual int32 GetBaseHealth() const override;
	
	virtual int32 GetMaxHealth() const override;

	virtual int32 GetCurrentHealth() const override;

	virtual int32 ModifyBaseHealth(int32 Value) override;

	virtual int32 ModifyMaxHealth(int32 Value) override;

	virtual int32 ModifyCurrentHealth(int32 Value) override;

	virtual void SetBaseHealth(int32 Value) override;

	virtual void SetMaxHealth(int32 Value) override;

	virtual void SetCurrentHealth(int32 Value) override;

	virtual bool IsLowOnHealth() const override;

	virtual int32 GetBaseMana() const override;

	virtual int32 GetMaxMana() const override;

	virtual int32 GetCurrentMana() const override;
	
	virtual int32 ModifyBaseMana(int32 Value) override;

	virtual int32 ModifyMaxMana(int32 Value) override;

	virtual int32 ModifyCurrentMana(int32 Value) override;

	virtual void SetBaseMana(int32 Value) override;

	virtual void SetMaxMana(int32 Value) override;

	virtual void SetCurrentMana(int32 Value) override;
	*/

	/** Returns 0 always */
	// virtual int32 GetBaseStamina() const override;
	
	/** Returns 0 always */
	// virtual int32 GetMaxStamina() const override;
	
	/** Returns 0 always */
	// virtual int32 GetCurrentStamina() const override;
	
	/** Override for PURE_VIRTUAL function in BaseStatsComponent. Does nothing */
	// virtual int32 ModifyBaseStamina(int32 Value) override;
	
	/** Override for PURE_VIRTUAL function in BaseStatsComponent. Does nothing */
	// virtual int32 ModifyMaxStamina(int32 Value) override;
	
	/** Override for PURE_VIRTUAL function in BaseStatsComponent. Does nothing */
	// virtual int32 ModifyCurrentStamina(int32 Value) override;

	/*
	virtual void SetBaseStamina(int32 Value) override;

	virtual void SetMaxStamina(int32 Value) override;

	virtual void SetCurrentStamina(int32 Value) override;
	*/

	virtual int32 GetHealthRegenRate() const override;

	virtual int32 ModifyHealthRegenRate(int32 Value) override;

	virtual void SetHealthRegenRate(int32 Value) override;

	virtual int32 GetManaRegenRate() const override;

	virtual int32 ModifyManaRegenRate(int32 Value) override;

	virtual void SetManaRegenRate(int32 Value) override;

	virtual int32 GetStaminaRegenRate() const override;

	virtual int32 ModifyStaminaRegenRate(int32 Value) override;

	virtual void SetStaminaRegenRate(int32 Value) override;

	virtual int32 GetPhysicalAttack() const override;

	virtual int32 GetMagickAttack() const override;

	virtual int32 ModifyPhysicalAttack(int32 Value) override;

	virtual int32 ModifyMagickAttack(int32 Value) override;

	virtual void SetPhysicalAttack(int32 Value) override;

	virtual void SetMagickAttack(int32 Value) override;

	virtual int32 GetPhysicalResistance() const override;

	virtual int32 GetMagickResistance() const override;

	virtual int32 ModifyPhysicalResistance(int32 Value) override;

	virtual int32 ModifyMagickResistance(int32 Value) override;

	virtual void SetPhysicalResistance(int32 Value) override;

	virtual void SetMagickResistance(int32 Value) override;

	virtual float GetPhysicalCritRate() const override;

	virtual float GetMagickCritRate() const override;

	virtual float ModifyPhysicalCritRate(float Value) override;

	virtual float ModifyMagickCritRate(float Value) override;

	virtual void SetPhysicalCritRate(float Value) override;

	virtual void SetMagickCritRate(float Value) override;

	virtual int32 GetPhysicalCritBonus() const override;

	virtual int32 GetMagickCritBonus() const override;

	virtual int32 ModifyPhysicalCritBonus(int32 Value) override;

	virtual int32 ModifyMagickCritBonus(int32 Value) override;

	virtual void SetPhysicalCritBonus(int32 Value) override;

	virtual void SetMagickCritBonus(int32 Value) override;

	virtual int32 GetElementalFireResistance() const override;

	virtual int32 GetElementalIceResistance() const override;

	virtual int32 GetElementalLightningResistance() const override;

	virtual int32 GetElementalPoisonResistance() const override;

	virtual int32 GetElementalHolyResistance() const override;

	virtual int32 GetElementalDarkResistance() const override;

	virtual int32 ModifyElementalFireResistance(int32 Value) override;

	virtual int32 ModifyElementalIceResistance(int32 Value) override;

	virtual int32 ModifyElementalLightningResistance(int32 Value) override;

	virtual int32 ModifyElementalPoisonResistance(int32 Value) override;

	virtual int32 ModifyElementalHolyResistance(int32 Value) override;

	virtual int32 ModifyElementalDarkResistance(int32 Value) override;

	virtual void SetElementalFireResistance(int32 Value) override;

	virtual void SetElementalIceResistance(int32 Value) override;

	virtual void SetElementalLightningResistance(int32 Value) override;

	virtual void SetElementalPoisonResistance(int32 Value) override;

	virtual void SetElementalHolyResistance(int32 Value) override;

	virtual void SetElementalDarkResistance(int32 Value) override;

	virtual int32 GetElementalFireDamage() const override;

	virtual int32 GetElementalIceDamage() const override;

	virtual int32 GetElementalLightningDamage() const override;

	virtual int32 GetElementalPoisonDamage() const override;

	virtual int32 GetElementalHolyDamage() const override;

	virtual int32 GetElementalDarkDamage() const override;

	virtual int32 ModifyElementalFireDamage(int32 Value) override;

	virtual int32 ModifyElementalIceDamage(int32 Value) override;

	virtual int32 ModifyElementalLightningDamage(int32 Value) override;

	virtual int32 ModifyElementalPoisonDamage(int32 Value) override;

	virtual int32 ModifyElementalHolyDamage(int32 Value) override;

	virtual int32 ModifyElementalDarkDamage(int32 Value) override;

	virtual void SetElementalFireDamage(int32 Value) override;

	virtual void SetElementalIceDamage(int32 Value) override;

	virtual void SetElementalLightningDamage(int32 Value) override;

	virtual void SetElementalPoisonDamage(int32 Value) override;

	virtual void SetElementalHolyDamage(int32 Value) override;

	virtual void SetElementalDarkDamage(int32 Value) override;

	virtual int32 GetBleedResistance() const override;

	virtual int32 GetCrowdControlResistance() const override;

	virtual int32 ModifyBleedResistance(int32 Value) override;

	virtual int32 ModifyCrowdControlResistance(int32 Value) override;

	virtual void SetBleedResistance(int32 Value) override;

	virtual void SetCrowdControlResistance(int32 Value) override;

	virtual void AddCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) override;

	virtual void AddCrowdControlImmunities(uint8 CCImmunities) override;

	virtual void RemoveCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) override;

	virtual void RemoveCrowdControlImmunities(uint8 CCImmunities) override;

	virtual void RemoveAllCrowdControlImmunities() override;

	virtual bool HasCrowdControlImmunity(ECrowdControlEffect CrowdControlEffect) const override;

	virtual uint8 GetCrowdControlImmunities() const override;

	virtual float GetCooldownModifier() const override;

	virtual float GetExpModifier() const override;

	virtual float GetDropRateModifier() const override;

	virtual float GetStaminaConsumptionModifier() const override;

	virtual float GetMovementSpeedModifier() const override;

	virtual float GetActiveTimeDilation() const override;

	virtual float GetSpellCastingSpeedModifier() const override;

	virtual float ModifyCooldownModifier(float Value) override;

	virtual float ModifyExpModifier(float Value) override;

	virtual float ModifyDropRateModifier(float Value) override;

	virtual float ModifyStaminaConsumptionModifier(float Value) override;

	virtual float ModifyMovementSpeedModifier(float Value) override;

	virtual float ModifyActiveTimeDilation(float Value) override;

	virtual float ModifySpellCastingSpeedModifier(float Value) override;

	virtual void SetCooldownModifier(float Value) override;

	virtual void SetExpModifier(float Value) override;

	virtual void SetDropRateModifier(float Value) override;

	virtual void SetStaminaConsumptionModifier(float Value) override;

	virtual void SetMovementSpeedModifier(float Value) override;

	virtual void SetActiveTimeDilation(float Value) override;

	virtual void SetSpellCastingSpeedModifier(float Value) override;

	virtual int32 GetDarkness() const override;

	virtual int32 ModifyDarkness(int32 Value) override;

	virtual void SetDarkness(int32 Value) override;

	virtual float GetPhysicalDamageReductionOnBlock() const override;

	virtual float GetMagickDamageReductionOnBlock() const override;

	virtual float ModifyPhysicalDamageReductionOnBlock(float Value) override;

	virtual float ModifyMagickDamageReductionOnBlock(float Value) override;

	virtual void SetPhysicalDamageReductionOnBlock(float Value) override;

	virtual void SetMagickDamageReductionOnBlock(float Value) override;

private:
	
	//~ @note All changes to variables similar to MaxHealth, CurrentHealth, etc. will occur ONLY on server and will automatically get replicated. No RPC needed.
	//~ @note Mana might be used to restrict AIs from spamming powerful spells/AOEs
	//~ @note AI characters do not have stamina
	
	//~ @todo Client RPCs to replicate the changes in BaseHealth, BaseMana, and BaseStamina to owning character

	/** Current character level */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 Level;

	/** Maximum health of character without any status effects */
	// UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	// int32 BaseHealth;

	/** Current maximum health of character - with or without any status effects */
	// UPROPERTY(Replicated)
	// int32 MaxHealth;

	/** Current health of character */
	// UPROPERTY(Replicated)
	// int32 CurrentHealth;
	
	/** Maximum mana of character without any status effects */
	// UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	// int32 BaseMana;

	/** Current maximum mana of character - with or without any status effects */
	// UPROPERTY(Replicated)
	// int32 MaxMana;

	/** Current mana of character */
	// UPROPERTY(Replicated)
	// int32 CurrentMana;
	
	//~ @note Assuming the regeneration rates for AI wouldn't change, they do not need to be replicated
	//~ If they do change, however, server should handle any changes in character health and mana

	UPROPERTY(EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 HealthRegenRate;
	
	UPROPERTY(EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 ManaRegenRate;

	//~ @note Since server will handle all calculations for damage, combat stats are not needed to be replicated to client

	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats)
	int32 PhysicalAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats)
	int32 PhysicalResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats)
	int32 MagickAttack;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats)
	int32 MagickResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats)
	float PhysicalCritRate;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats)
	float MagickCritRate;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats)
	int32 PhysicalCritBonus;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats)
	int32 MagickCritBonus;

	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalFireDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalIceDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalPoisonDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalLightningDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalHolyDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = OffensiveStats, AdvancedDisplay)
	int32 ElementalDarkDamage;

	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalFireResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalIceResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalPoisonResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalLightningResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalHolyResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 ElementalDarkResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 BleedResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = DefensiveStats, AdvancedDisplay)
	int32 CrowdControlResistance;
	
	UPROPERTY(EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float CooldownModifier;
	
	UPROPERTY(EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float ExpModifier;
	
	UPROPERTY(EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float DropRateModifier;
	
	//~ @note MovementSpeedModifier, AnimationSpeedModifier, and SpellCastingSpeedModifier will be replicated to all clients

	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float MovementSpeedModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float ActiveTimeDilation;
	
	UPROPERTY(EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float SpellCastingSpeedModifier;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats)
	int32 Darkness;

	AAICharacterBase* OwningAIChar;
	
};
