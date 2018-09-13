// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/Components/StatsComponentBase.h"
#include "PlayerStatsComponent.generated.h"

class APlayerCharacter;

/**
 * PlayerStatsComponent is used to manage stats of a player controlled character
 */
UCLASS()
class EOD_API UPlayerStatsComponent : public UStatsComponentBase
{
	GENERATED_BODY()
	
public:

	UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer);

	/** Initialize current health, mana, and stamina */
	virtual void PostInitProperties() override;

	virtual void BeginPlay() override;

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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
	
	virtual int32 GetBaseStamina() const override;

	virtual int32 GetMaxStamina() const override;

	virtual int32 GetCurrentStamina() const override;
	
	virtual int32 ModifyBaseStamina(int32 Value) override;

	virtual int32 ModifyMaxStamina(int32 Value) override;

	virtual int32 ModifyCurrentStamina(int32 Value) override;

	virtual void SetBaseStamina(int32 Value) override;

	virtual void SetMaxStamina(int32 Value) override;

	virtual void SetCurrentStamina(int32 Value) override;

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

	//~ @todo Multicast for replicating level changes
	//~ @todo Client RPCs to replicate the changes in BaseHealth, BaseMana, and BaseStamina to owning character

	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 Level;

	/** Maximum health of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseHealth;

	/** Current maximum health of character - with or without any status effects */
	UPROPERTY(Replicated)
	int32 MaxHealth;

	/** Current health of character */
	UPROPERTY(Replicated)
	int32 CurrentHealth;
	
	/** Maximum mana of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseMana;
	
	/** Current maximum health of character - with or without any status effects */
	UPROPERTY(Replicated)
	int32 MaxMana;
	
	/** Current mana of character */
	UPROPERTY(Replicated)
	int32 CurrentMana;
	
	/** Maximum stamina of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseStamina;
	
	/** Current maximum stamina of character - with or without any status effects */
	UPROPERTY(Replicated)
	int32 MaxStamina;
	
	/** Current stamina of character */
	UPROPERTY(Replicated)
	int32 CurrentStamina;

	//~ If regeneration rates are not displayed to player in-game then they are not needed to be replicated
	//~ However, if we do need to replicate regeneration rate then we can simply replicate it to owner only

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

	//~ @note Properties like ElementalResistance may not need to be replicated.
	//~ The server can simply do the calculations with it's own copy of ElementalResistance
	//~ and pass on the result to the clients

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
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	//~ @note CooldownModifier, ExpModifier, DropRateModifier, and StaminaConsumptionModifier
	//~ will be replicated to owner only. They are irrelevant to other clients.

	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float CooldownModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float ExpModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float DropRateModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float StaminaConsumptionModifier;

	//~ @note MovementSpeedModifier, AnimationSpeedModifier, SpellCastingSpeedModifier,
	//~ and Darkness will be replicated to all clients

	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float MovementSpeedModifier;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float ActiveTimeDilation;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	float SpellCastingSpeedModifier;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats)
	int32 Darkness;

	UPROPERTY(Transient)
	float PhysicalDamageReductionOnBlock;

	UPROPERTY(Transient)
	float MagickDamageReductionOnBlock;

	UPROPERTY(EditDefaultsOnly, Category = Regeneration)
	float HealthRegenTickInterval;

	UPROPERTY(EditDefaultsOnly, Category = Regeneration)
	float ManaRegenTickInterval;

	UPROPERTY(EditDefaultsOnly, Category = Regeneration)
	float StaminaRegenTickInterval;

	UPROPERTY(Transient)
	bool bIsRegeneratingHealth;

	UPROPERTY(Transient)
	bool bIsRegeneratingMana;

	UPROPERTY(Transient)
	bool bIsRegeneratingStamina;

	FTimerHandle HealthRegenTimerHandle;

	FTimerHandle ManaRegenTimerHandle;

	FTimerHandle StaminaRegenTimerHandle;

	APlayerCharacter* OwningPlayer;

	/** Starts health regeneration on player. Automatically stops once the health is full or if manually stopped */
	void ActivateHealthRegeneration();

	/** Starts health regeneration on player. Automatically stops once the mana is full or if manually stopped */
	void ActivateManaRegeneration();

	//~ @note maybe its better to never stop stamina regeneration
	/** Starts health regeneration on player. Automatically stops once the stamina is full or if manually stopped */
	void ActivateStaminaRegeneration();

	/** Stops health regeneration */
	void DeactivateHealthRegeneration();

	/** Stops mana regeneration */
	void DeactivateManaRegeneration();

	/** Stops stamina regeneration */
	void DeactivateStaminaRegeneration();

	void RegenerateHealth();

	void RegenerateMana();

	void RegenerateStamina();

};
