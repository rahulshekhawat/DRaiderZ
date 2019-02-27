// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/WeaponLibrary.h"
#include "EOD/Characters/Components/StatsComponentBase.h"

#include "PlayerStatsComponent.generated.h"

class APlayerCharacter;
class UStatusIndicatorWidget;

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

	virtual int32 ModifyHealthRegenRate(int32 Value) override;

	virtual void SetHealthRegenRate(int32 Value) override;

	virtual int32 ModifyManaRegenRate(int32 Value) override;

	virtual void SetManaRegenRate(int32 Value) override;

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

	// virtual float GetMovementSpeedModifier() const override;

	virtual float GetActiveTimeDilation() const override;
	
	virtual float GetSpellCastingSpeedModifier() const override;
	
	virtual float ModifyCooldownModifier(float Value) override;
	
	virtual float ModifyExpModifier(float Value) override;

	virtual float ModifyDropRateModifier(float Value) override;
	
	virtual float ModifyStaminaConsumptionModifier(float Value) override;
	
	// virtual float ModifyMovementSpeedModifier(float Value) override;
	
	virtual float ModifyActiveTimeDilation(float Value) override;
	
	virtual float ModifySpellCastingSpeedModifier(float Value) override;

	virtual void SetCooldownModifier(float Value) override;
	
	virtual void SetExpModifier(float Value) override;
	
	virtual void SetDropRateModifier(float Value) override;
	
	virtual void SetStaminaConsumptionModifier(float Value) override;
	
	// virtual void SetMovementSpeedModifier(float Value) override;
	
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

	FORCEINLINE void InitializeComponentWidget();

	void AddPrimaryWeaponStats(FWeaponTableRow* WeaponData);

	void AddSecondaryWeaponStats(FWeaponTableRow* WeaponData);

	void RemovePrimaryWeaponStats();

	void RemoveSecondaryWeaponStats();

	virtual void AddCrowdControlImmunitiesFromSkill(uint8 CCImmunities) override;

	virtual void RemoveCrowdControlImmunitiesFromSkil() override;

	virtual uint8 GetCrowdControlImmunitiesFromSkill() const;

private:
	
	//~ @note All changes to variables similar to MaxHealth, CurrentHealth, etc. will occur ONLY on server and will automatically get replicated. No RPC needed.

	//~ @todo Multicast for replicating level changes
	//~ @todo Client RPCs to replicate the changes in BaseHealth, BaseMana, and BaseStamina to owning character

	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 Level;

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

	UPROPERTY(Replicated, EditDefaultsOnly, Category = DefensiveStats, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunitiesFromSkill;

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

	// UPROPERTY(Replicated, EditDefaultsOnly, Category = AdditionalStats, AdvancedDisplay)
	// float MovementSpeedModifier;
	
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

	UPROPERTY(Transient)
	UStatusIndicatorWidget* StatusIndicatorWidget;

	UPROPERTY(EditAnywhere, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStatusIndicatorWidget> StatusIndicatorWidgetClass;

	FWeaponTableRow* PrimaryWeaponData;

	FWeaponTableRow* SecondaryWeaponData;

	/*
	void RegenerateHealth();

	void RegenerateMana();

	void RegenerateStamina();
	*/

};
