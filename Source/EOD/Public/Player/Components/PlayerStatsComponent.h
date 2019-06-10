// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArmorLibrary.h"
#include "WeaponLibrary.h"
#include "StatsComponentBase.h"

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

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer);

	/** Initialize current health, mana, and stamina */
	virtual void PostInitProperties() override;

	virtual void BeginPlay() override;

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------------------------------
	//  
	// --------------------------------------



	FORCEINLINE void InitializeComponentWidget();

	void AddPrimaryWeaponStats(FWeaponTableRow* WeaponData);

	void AddSecondaryWeaponStats(FWeaponTableRow* WeaponData);

	void RemovePrimaryWeaponStats();

	void RemoveSecondaryWeaponStats();

	void AddArmorStats(FArmorTableRow* ArmorData);
	void RemoveArmorStats(EArmorType ArmorType);

private:
	
	//~ @note All changes to variables similar to MaxHealth, CurrentHealth, etc. will occur ONLY on server and will automatically get replicated. No RPC needed.

	//~ @todo Multicast for replicating level changes
	//~ @todo Client RPCs to replicate the changes in BaseHealth, BaseMana, and BaseStamina to owning character

	/*
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
	*/

	FWeaponTableRow* PrimaryWeaponData;
	FWeaponTableRow* SecondaryWeaponData;

	TMap<EArmorType, FArmorTableRow*> ArmorsData;
	

};
