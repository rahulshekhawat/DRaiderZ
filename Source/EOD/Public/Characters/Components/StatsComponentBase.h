// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatLibrary.h"

#include "Components/ActorComponent.h"
#include "StatsComponentBase.generated.h"


/** Delegate for whenever one of the health, mana, or stamina changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatChangedMCDelegate, int32, BaseStatValue, int32, MaxStatValue, int32, CurrentStatValue);


/**
 * An abstract base class that lays out the expected behavior of stats component to manage character stats.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API UStatsComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Sets default values for this component's properties */
	UStatsComponentBase(const FObjectInitializer& ObjectInitializer);

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --------------------------------------
	//  Health
	// --------------------------------------	

	UPROPERTY(BlueprintAssignable, Category = "Stats Component")
	FOnStatChangedMCDelegate OnHealthChanged;

	FORCEINLINE bool IsLowOnHealth() const;

	FORCEINLINE int32 GetBaseHealth() const;

	FORCEINLINE int32 GetMaxHealth() const;

	FORCEINLINE int32 GetCurrentHealth() const;

	inline void ModifyBaseHealth(int32 Value, bool bPercent = false);

	inline void ModifyMaxHealth(int32 Value, bool bPercent = false);

	inline void ModifyCurrentHealth(int32 Value, bool bPercent = false);

protected:

	/** Maximum health of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseHealth;

	/** Percentage (0 to 1) of max health that will be considered low health */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	float LowHealthPercent;

	/** Current maximum health of character (with or without any status effects) */
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	int32 MaxHealth;

	/** Current health of character */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	int32 CurrentHealth;

private:

	FORCEINLINE void SetBaseHealth(int32 Value);

	FORCEINLINE void SetMaxHealth(int32 Value);

	FORCEINLINE void SetCurrentHealth(int32 Value);

public:

	// --------------------------------------
	//  Mana
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, Category = "Stats Component")
	FOnStatChangedMCDelegate OnManaChanged;

	FORCEINLINE int32 GetBaseMana() const;

	FORCEINLINE int32 GetMaxMana() const;

	FORCEINLINE int32 GetCurrentMana() const;

	inline void ModifyBaseMana(int32 Value, bool bPercent = false);

	inline void ModifyMaxMana(int32 Value, bool bPercent = false);

	inline void ModifyCurrentMana(int32 Value, bool bPercent = false);

protected:

	/** Maximum mana of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseMana;

	/** Current maximum mana of character - with or without any status effects */
	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	int32 MaxMana;

	/** Current mana of character */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentMana)
	int32 CurrentMana;

private:

	FORCEINLINE void SetBaseMana(int32 Value);

	FORCEINLINE void SetMaxMana(int32 Value);

	FORCEINLINE void SetCurrentMana(int32 Value);

public:

	// --------------------------------------
	//  Stamina
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, Category = "Stats Component")
	FOnStatChangedMCDelegate OnStaminaChanged;

	FORCEINLINE int32 GetBaseStamina() const;

	FORCEINLINE int32 GetMaxStamina() const;

	FORCEINLINE int32 GetCurrentStamina() const;

	inline void ModifyBaseStamina(int32 Value, bool bPercent = false);

	inline void ModifyMaxStamina(int32 Value, bool bPercent = false);

	inline void ModifyCurrentStamina(int32 Value, bool bPercent = false);

protected:

	/** Maximum mana of character without any status effects */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	int32 BaseStamina;

	/** Current maximum stamina of character - with or without any status effects */
	UPROPERTY(ReplicatedUsing = OnRep_MaxStamina)
	int32 MaxStamina;

	/** Current stamina of character */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentStamina)
	int32 CurrentStamina;

private:

	FORCEINLINE void SetBaseStamina(int32 Value);

	FORCEINLINE void SetMaxStamina(int32 Value);

	FORCEINLINE void SetCurrentStamina(int32 Value);

public:

	// --------------------------------------
	//  Regeneration
	// --------------------------------------

	FORCEINLINE int32 GetHealthRegenRate() const;

	FORCEINLINE int32 GetManaRegenRate() const;

	FORCEINLINE int32 GetStaminaRegenRate() const;

	inline void ModifyHealthRegenRate(int32 Value);

	inline void ModifyManaRegenRate(int32 Value);

	inline void ModifyStaminaRegenRate(int32 Value);

protected:

	/** Determines whether this character can regenerate health at all */
	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
	uint32 bHasHealthRegenration : 1;

	/** Determines whether this character can regenerate mana at all */
	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
	uint32 bHasManaRegenration : 1;

	/** Determines whether this character can regenerate stamina at all */
	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
	uint32 bHasStaminaRegenration : 1;

	/** Determines whether this character is currently regenerating health */
	UPROPERTY(Transient)
	uint32 bIsRegeneratingHealth : 1;

	/** Determines whether this character is currently regenerating mana */
	UPROPERTY(Transient)
	uint32 bIsRegeneratingMana : 1;

	/** Determines whether this character is currently regenerating stamina */
	UPROPERTY(Transient)
	uint32 bIsRegeneratingStamina : 1;

	/** Delay between health regeneration update */
	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
	float HealthRegenTickInterval;

	/** Delay between mana regeneration update */
	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
	float ManaRegenTickInterval;

	/** Delay between stamina regeneration update */
	UPROPERTY(EditDefaultsOnly, Category = "Regeneration")
	float StaminaRegenTickInterval;

	//~ @note If regeneration rates are not displayed to player in-game then they are not needed to be replicated

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 HealthRegenRate;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 ManaRegenRate;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	int32 StaminaRegenRate;

	FTimerHandle HealthRegenTimerHandle;

	FTimerHandle ManaRegenTimerHandle;

	FTimerHandle StaminaRegenTimerHandle;

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

private:

	void RegenerateHealth();

	void RegenerateMana();

	void RegenerateStamina();

	inline void SetHealthRegenRate(int32 Value);

	inline void SetManaRegenRate(int32 Value);

	inline void SetStaminaRegenRate(int32 Value);

public:

	// --------------------------------------
	//  Attack
	// --------------------------------------

	virtual int32 GetPhysicalAttack() const { return 0; }

	virtual int32 GetMagickAttack() const { return 0; }

	// virtual int32 ModifyPhysicalAttack


	// virtual int32 ModifyPhysicalAttack(int32 Value, bool bPercent = false);
	virtual int32 ModifyPhysicalAttack(int32 Value);

	// virtual int32 ModifyMagickAttack(int32 Value, bool bPercent = false);
	virtual int32 ModifyMagickAttack(int32 Value);

	virtual void SetPhysicalAttack(int32 Value);

	virtual void SetMagickAttack(int32 Value);

protected:

	/** Maximum mana of character without any status effects */
	// UPROPERTY(EditDefaultsOnly, Category = BaseStats)
		// int32 BaseStamina;

	/** Current maximum stamina of character - with or without any status effects */
	// UPROPERTY(ReplicatedUsing = OnRep_MaxStamina)
		// int32 MaxStamina;

	/** Current stamina of character */
	// UPROPERTY(ReplicatedUsing = OnRep_CurrentStamina)
		// int32 CurrentStamina;




private:



public:


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

	virtual float GetActiveTimeDilation() const PURE_VIRTUAL(UStatsComponentBase::GetActiveTimeDilation, return 0.f; );
	
	virtual float GetSpellCastingSpeedModifier() const PURE_VIRTUAL(UStatsComponentBase::GetSpellCastingSpeedModifier, return 0.f; );
	
	virtual float ModifyCooldownModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyCooldownModifier, return 0.f; );
	
	virtual float ModifyExpModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyExpModifier, return 0.f; );

	virtual float ModifyDropRateModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyDropRateModifier, return 0.f; );
	
	virtual float ModifyStaminaConsumptionModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyStaminaConsumptionModifier, return 0.f; );
	
	virtual float ModifyActiveTimeDilation(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifyActiveTimeDilation, return 0.f; );
	
	virtual float ModifySpellCastingSpeedModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::ModifySpellCastingSpeedModifier, return 0.f; );

	virtual void SetCooldownModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetCooldownModifier, );
	
	virtual void SetExpModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetExpModifier, );
	
	virtual void SetDropRateModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetDropRateModifier, );
	
	virtual void SetStaminaConsumptionModifier(float Value) PURE_VIRTUAL(UStatsComponentBase::SetStaminaConsumptionModifier, );
	
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

	virtual void AddCrowdControlImmunitiesFromSkill(uint8 CCImmunities) PURE_VIRTUAL(UStatsComponentBase::AddCrowdControlImmunitiesFromSkill, );

	virtual void RemoveCrowdControlImmunitiesFromSkil() PURE_VIRTUAL(UStatsComponentBase::RemoveCrowdControlImmunitiesFromSkil, );

	virtual uint8 GetCrowdControlImmunitiesFromSkill() const PURE_VIRTUAL(UStatsComponentBase::GetCrowdControlImmunitiesFromSkill, return 0; );

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_CurrentHealth();

	UFUNCTION()
	virtual void OnRep_MaxMana();

	UFUNCTION()
	virtual void OnRep_CurrentMana();

	UFUNCTION()
	virtual void OnRep_MaxStamina();

	UFUNCTION()
	virtual void OnRep_CurrentStamina();

};

FORCEINLINE bool UStatsComponentBase::IsLowOnHealth() const
{
	float CurrentPercent = (float)CurrentHealth / (float)MaxHealth;
	return CurrentPercent <= LowHealthPercent;
}

FORCEINLINE int32 UStatsComponentBase::GetBaseHealth() const
{
	return BaseHealth;
}

FORCEINLINE int32 UStatsComponentBase::GetMaxHealth() const
{
	return MaxHealth;
}

FORCEINLINE int32 UStatsComponentBase::GetCurrentHealth() const
{
	return CurrentHealth;
}

inline void UStatsComponentBase::ModifyBaseHealth(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(BaseHealth * Value) / 100.f;
		SetBaseHealth(BaseHealth + ModificationValue);
	}
	else
	{
		SetBaseHealth(BaseHealth + Value);
	}
}

inline void UStatsComponentBase::ModifyMaxHealth(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(MaxHealth * Value) / 100.f;
		SetMaxHealth(MaxHealth + ModificationValue);
	}
	else
	{
		SetMaxHealth(MaxHealth + Value);
	}
}

inline void UStatsComponentBase::ModifyCurrentHealth(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(CurrentHealth * Value) / 100.f;
		SetCurrentHealth(CurrentHealth + ModificationValue);
	}
	else
	{
		SetCurrentHealth(CurrentHealth + Value);
	}
}

FORCEINLINE void UStatsComponentBase::SetBaseHealth(int32 Value)
{
	BaseHealth = Value <= 0 ? 0 : Value;
	OnHealthChanged.Broadcast(BaseHealth, MaxHealth, CurrentHealth);
}

FORCEINLINE void UStatsComponentBase::SetMaxHealth(int32 Value)
{
	MaxHealth = Value <= 0 ? 0 : Value;
	OnHealthChanged.Broadcast(BaseHealth, MaxHealth, CurrentHealth);
}

FORCEINLINE void UStatsComponentBase::SetCurrentHealth(int32 Value)
{
	CurrentHealth = Value <= 0 ? 0 : Value;
	OnHealthChanged.Broadcast(BaseHealth, MaxHealth, CurrentHealth);
	if (bHasHealthRegenration && CurrentHealth < MaxHealth && !bIsRegeneratingHealth)
	{
		ActivateHealthRegeneration();
	}
}

FORCEINLINE int32 UStatsComponentBase::GetBaseMana() const
{
	return BaseMana;
}

FORCEINLINE int32 UStatsComponentBase::GetMaxMana() const
{
	return MaxMana;
}

FORCEINLINE int32 UStatsComponentBase::GetCurrentMana() const
{
	return CurrentMana;
}

inline void UStatsComponentBase::ModifyBaseMana(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(BaseMana * Value) / 100.f;
		SetBaseMana(BaseMana + ModificationValue);
	}
	else
	{
		SetBaseMana(BaseMana + Value);
	}
}

inline void UStatsComponentBase::ModifyMaxMana(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(MaxMana * Value) / 100.f;
		SetMaxMana(MaxMana + ModificationValue);
	}
	else
	{
		SetMaxMana(MaxMana + Value);
	}
}

inline void UStatsComponentBase::ModifyCurrentMana(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(CurrentMana * Value) / 100.f;
		SetCurrentMana(CurrentMana + ModificationValue);
	}
	else
	{
		SetCurrentMana(CurrentMana + Value);
	}
}

FORCEINLINE void UStatsComponentBase::SetBaseMana(int32 Value)
{
	BaseMana = Value <= 0 ? 0 : Value;
	OnManaChanged.Broadcast(BaseMana, MaxMana, CurrentMana);
}

FORCEINLINE void UStatsComponentBase::SetMaxMana(int32 Value)
{
	MaxMana = Value <= 0 ? 0 : Value;
	OnManaChanged.Broadcast(BaseMana, MaxMana, CurrentMana);
}

FORCEINLINE void UStatsComponentBase::SetCurrentMana(int32 Value)
{
	CurrentMana = Value <= 0 ? 0 : Value;
	OnManaChanged.Broadcast(BaseMana, MaxMana, CurrentMana);
	if (bHasManaRegenration && CurrentMana < MaxMana && !bIsRegeneratingMana)
	{
		ActivateManaRegeneration();
	}
}

FORCEINLINE int32 UStatsComponentBase::GetBaseStamina() const
{
	return BaseStamina;
}

FORCEINLINE int32 UStatsComponentBase::GetMaxStamina() const
{
	return MaxStamina;
}

FORCEINLINE int32 UStatsComponentBase::GetCurrentStamina() const
{
	return CurrentStamina;
}

inline void UStatsComponentBase::ModifyBaseStamina(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(BaseStamina * Value) / 100.f;
		SetBaseStamina(BaseStamina + ModificationValue);
	}
	else
	{
		SetBaseStamina(BaseStamina + Value);
	}
}

inline void UStatsComponentBase::ModifyMaxStamina(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(MaxStamina * Value) / 100.f;
		SetMaxStamina(MaxStamina + ModificationValue);
	}
	else
	{
		SetMaxStamina(MaxStamina + Value);
	}
}

inline void UStatsComponentBase::ModifyCurrentStamina(int32 Value, bool bPercent)
{
	if (Value == 0)
	{
		return;
	}

	if (bPercent)
	{
		int32 ModificationValue = (float)(CurrentStamina * Value) / 100.f;
		SetCurrentStamina(CurrentStamina + ModificationValue);
	}
	else
	{
		SetCurrentStamina(CurrentStamina + Value);
	}
}

FORCEINLINE void UStatsComponentBase::SetBaseStamina(int32 Value)
{
	BaseStamina = Value <= 0 ? 0 : Value;
	OnStaminaChanged.Broadcast(BaseStamina, MaxStamina, CurrentStamina);
}

FORCEINLINE void UStatsComponentBase::SetMaxStamina(int32 Value)
{
	MaxStamina = Value <= 0 ? 0 : Value;
	OnStaminaChanged.Broadcast(BaseStamina, MaxStamina, CurrentStamina);
}

FORCEINLINE void UStatsComponentBase::SetCurrentStamina(int32 Value)
{
	CurrentStamina = Value <= 0 ? 0 : Value;
	OnStaminaChanged.Broadcast(BaseStamina, MaxStamina, CurrentStamina);
	if (bHasStaminaRegenration && CurrentStamina < MaxStamina && !bIsRegeneratingStamina)
	{
		ActivateStaminaRegeneration();
	}
}

FORCEINLINE int32 UStatsComponentBase::GetHealthRegenRate() const
{
	return HealthRegenRate;
}

FORCEINLINE int32 UStatsComponentBase::GetManaRegenRate() const
{
	return ManaRegenRate;
}

FORCEINLINE int32 UStatsComponentBase::GetStaminaRegenRate() const
{
	return StaminaRegenRate;
}

inline void UStatsComponentBase::ModifyHealthRegenRate(int32 Value)
{
	SetHealthRegenRate(HealthRegenRate + Value);
}

inline void UStatsComponentBase::ModifyManaRegenRate(int32 Value)
{
	SetManaRegenRate(ManaRegenRate + Value);
}

inline void UStatsComponentBase::ModifyStaminaRegenRate(int32 Value)
{
	SetStaminaRegenRate(StaminaRegenRate + Value);
}

inline void UStatsComponentBase::SetHealthRegenRate(int32 Value)
{
	HealthRegenRate = Value;
}

inline void UStatsComponentBase::SetManaRegenRate(int32 Value)
{
	ManaRegenRate = Value;
}

inline void UStatsComponentBase::SetStaminaRegenRate(int32 Value)
{
	StaminaRegenRate = Value;
}
