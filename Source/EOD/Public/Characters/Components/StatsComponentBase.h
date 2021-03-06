// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatLibrary.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "StatsComponentBase.generated.h"


/**
 * Delegate for when the primary stat value changes
 * @param1 int32 NewMaxValue
 * @param2 int32 NewCurrentValue
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPrimaryStatChangedMCDelegate, int32, int32);

/**
 * Delegate for when the secondary stat value changes
 * @param1 float NewValue
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGenericStatChangedMCDelegate, float);

UENUM(BlueprintType)
enum class EStatModType : uint8
{
	Flat,
	Percent
};

USTRUCT(BlueprintType)
struct EOD_API FStatModifier
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Value;

	UPROPERTY()
	EStatModType ModType;

	FStatModifier() :
		Value(0.f),
		ModType(EStatModType::Flat)
	{
	}

	FStatModifier(float InValue, EStatModType InModType) :
		Value(InValue),
		ModType(InModType)
	{
	}

	FORCEINLINE bool operator==(const FStatModifier& Other) const
	{
		return this->Value == Other.Value && this->ModType == Other.ModType;
	}
	FORCEINLINE bool operator!=(const FStatModifier& Other) const
	{
		return this->Value != Other.Value || this->ModType != Other.ModType;
	}

	/**
	 * Comparison operator used for sorting based on ModOrder.
	 */
	FORCEINLINE bool operator<(const FStatModifier& Other) const
	{
		return this->ModType < Other.ModType;
	}

	/**
	 * Comparison operator used for sorting based on ModOrder.
	 */
	FORCEINLINE bool operator>(const FStatModifier& Other) const
	{
		return this->ModType > Other.ModType;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FPrimaryStat
{
	GENERATED_USTRUCT_BODY()

public:

	~FPrimaryStat() { ; }

	FPrimaryStat() :
		MaxValue_NoMod(1),
		MaxValue(1),
		CurrentValue(0)
	{
	}

	FPrimaryStat(int32 InMaxValue, int32 InCurrentValue)
	{
		SetMaxValue(InMaxValue);
		SetCurrentValue(InCurrentValue);
	}

public:

	/** Directly set the maximum value that doesn't have any modifier applied */
	void SetMaxValue(int32 InValue)
	{
		InValue = InValue <= 0 ? 1 : InValue;
		if (MaxValue_NoMod != InValue)
		{
			MaxValue_NoMod = InValue;
			RecalculateMaxValue();
			OnStatValueChanged.Broadcast(MaxValue, CurrentValue);
		}
	}

	void ModifyCurrentValue(int32 InValue, bool bPercent = false)
	{
		if (InValue == 0)
		{
			return;
		}

		if (bPercent)
		{
			int32 ModValue = (float)(CurrentValue * InValue) / 100.f;
			SetCurrentValue(CurrentValue + ModValue);
		}
		else
		{
			SetCurrentValue(CurrentValue + InValue);
		}
	}

	void RefillCurrentValue()
	{
		int32 Max = GetMaxValue();
		SetCurrentValue(Max);
	}

	void SetCurrentValue(int32 InValue)
	{
		int32 Max = GetMaxValue();
		CurrentValue = InValue <= 0 ? 0 : InValue >= Max ? Max : InValue;
		OnStatValueChanged.Broadcast(Max, CurrentValue);
	}

	int32 GetMaxValue() const { return MaxValue; }

	int32 GetCurrentValue() const { return CurrentValue; }

	/** Add a modifier to the maximum value of this primary stat */
	void AddModifier(UObject const * const SourceObj, const FStatModifier& NewMod)
	{
		if (SourceObj)
		{
			uint32 UniqueID = SourceObj->GetUniqueID();
			if (Modifiers.Contains(UniqueID))
			{
				Modifiers[UniqueID] = NewMod;
			}
			else
			{
				Modifiers.Add(UniqueID, NewMod);
			}
			RecalculateMaxValue();
			OnStatValueChanged.Broadcast(MaxValue, CurrentValue);
		}
	}

	/** Remove a modifier from the maximum value of this primary stat */
	void RemoveModifier(UObject const * const SourceObj)
	{
		if (SourceObj)
		{
			uint32 UniqueID = SourceObj->GetUniqueID();
			if (Modifiers.Contains(UniqueID))
			{
				Modifiers.Remove(UniqueID);
				RecalculateMaxValue();
				OnStatValueChanged.Broadcast(MaxValue, CurrentValue);
			}
		}
	}

	void ForceBroadcastDelegate()
	{
		int32 CV = GetCurrentValue();
		int32 MV = GetMaxValue();
		OnStatValueChanged.Broadcast(MV, CV);
	}

	FOnPrimaryStatChangedMCDelegate OnStatValueChanged;

private:

	int32 RecalculateMaxValue()
	{
		Modifiers.ValueSort([&](const FStatModifier& Mod1, const FStatModifier& Mod2) { return Mod1 < Mod2; });

		int32 MaxFlat = MaxValue_NoMod;
		float FlatPercent = 0.f;

		for (const TPair<uint32, FStatModifier>& ModPair : Modifiers)
		{
			if (ModPair.Value.ModType == EStatModType::Flat)
			{
				MaxFlat += ModPair.Value.Value;
			}
			else if (ModPair.Value.ModType == EStatModType::Percent)
			{
				FlatPercent += ModPair.Value.Value;
			}
		}

		MaxValue = MaxFlat + (MaxFlat * (FlatPercent / 100.f));
		return MaxValue;
	}

	int32 MaxValue_NoMod;

	UPROPERTY()
	int32 MaxValue;

	UPROPERTY()
	int32 CurrentValue;

	UPROPERTY()
	TMap<uint32, FStatModifier> Modifiers;
};

USTRUCT(BlueprintType)
struct EOD_API FGenericStat
{
	GENERATED_USTRUCT_BODY()

public:

	~FGenericStat() { ; }

	FGenericStat() :
		Value_NoMod(0),
		Value(0)
	{
	}

	FGenericStat(float InValue)
	{
		SetValue(InValue);
	}

public:

	/** Directly set the value that doesn't have any modifier applied */
	void SetValue(float InValue)
	{
		InValue = InValue <= 0 ? 1 : InValue;
		if (Value_NoMod != InValue)
		{
			Value_NoMod = InValue;
			RecalculateValue();
			OnStatValueChanged.Broadcast(Value);
		}
	}

	float GetValue() { return Value; }

	/** Add a modifier to the maximum value of this primary stat */
	void AddModifier(UObject const* const SourceObj, const FStatModifier& NewMod)
	{
		if (SourceObj)
		{
			uint32 UniqueID = SourceObj->GetUniqueID();
			if (Modifiers.Contains(UniqueID))
			{
				Modifiers[UniqueID] = NewMod;
			}
			else
			{
				Modifiers.Add(UniqueID, NewMod);
			}
			RecalculateValue();
			OnStatValueChanged.Broadcast(Value);
		}
	}

	/** Remove a modifier from the maximum value of this primary stat */
	void RemoveModifier(UObject const* const SourceObj)
	{
		if (SourceObj)
		{
			uint32 UniqueID = SourceObj->GetUniqueID();
			if (Modifiers.Contains(UniqueID))
			{
				Modifiers.Remove(UniqueID);
				RecalculateValue();
				OnStatValueChanged.Broadcast(Value);
			}
		}
	}

	void ForceBroadcastDelegate()
	{
		float CV = GetValue();
		OnStatValueChanged.Broadcast(CV);
	}

	FOnGenericStatChangedMCDelegate OnStatValueChanged;

private:

	float RecalculateValue()
	{
		Modifiers.ValueSort([&](const FStatModifier& Mod1, const FStatModifier& Mod2) { return Mod1 < Mod2; });

		int32 MaxFlat = Value_NoMod;
		float FlatPercent = 0.f;

		for (const TPair<uint32, FStatModifier>& ModPair : Modifiers)
		{
			if (ModPair.Value.ModType == EStatModType::Flat)
			{
				MaxFlat += ModPair.Value.Value;
			}
			else if (ModPair.Value.ModType == EStatModType::Percent)
			{
				FlatPercent += ModPair.Value.Value;
			}
		}

		Value = MaxFlat + (MaxFlat * (FlatPercent / 100.f));
		return Value;
	}

	float Value_NoMod;

	UPROPERTY()
	float Value;

	UPROPERTY()
	TMap<uint32, FStatModifier> Modifiers;

};

UENUM(BlueprintType)
enum class ECCImmunityModType : uint8
{
	Additive,
	Reductive
};

USTRUCT(BlueprintType)
struct EOD_API FCCImmunityModifier
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 Value;

	UPROPERTY()
	ECCImmunityModType ModType;

	FCCImmunityModifier() :
		Value(0),
		ModType(ECCImmunityModType::Additive)
	{
	}

	FCCImmunityModifier(uint8 InValue, ECCImmunityModType InModType) :
		Value(InValue),
		ModType(InModType)
	{
	}

	FORCEINLINE bool operator==(const FCCImmunityModifier& Other) const
	{
		return this->Value == Other.Value && this->ModType == Other.ModType;
	}
	FORCEINLINE bool operator!=(const FCCImmunityModifier& Other) const
	{
		return this->Value != Other.Value || this->ModType != Other.ModType;
	}

	/**
	 * Comparison operator used for sorting based on ModOrder.
	 */
	FORCEINLINE bool operator<(const FCCImmunityModifier& Other) const
	{
		return this->ModType < Other.ModType;
	}

	/**
	 * Comparison operator used for sorting based on ModOrder.
	 */
	FORCEINLINE bool operator>(const FCCImmunityModifier& Other) const
	{
		return this->ModType > Other.ModType;
	}
};

USTRUCT(BlueprintType)
struct EOD_API FCCImmunities
{
	GENERATED_USTRUCT_BODY()

public:

	~FCCImmunities() { ; }

	FCCImmunities() :
		Value_NoMod(0),
		Value(0)
	{
	}

	FCCImmunities(float InValue)
	{
		SetValue(InValue);
	}

public:

	/** Directly set the value that doesn't have any modifier applied */
	void SetValue(float InValue)
	{
		Value_NoMod = InValue;
	}

	float GetValue() { return Value; }

	/** Add a modifier to the maximum value of this primary stat */
	void AddModifier(UObject const* const SourceObj, const FCCImmunityModifier& NewMod)
	{
		if (SourceObj)
		{
			uint32 UniqueID = SourceObj->GetUniqueID();
			if (Modifiers.Contains(UniqueID))
			{
				Modifiers[UniqueID] = NewMod;
			}
			else
			{
				Modifiers.Add(UniqueID, NewMod);
			}
			RecalculateValue();
		}
	}

	/** Remove a modifier from the maximum value of this primary stat */
	void RemoveModifier(UObject const* const SourceObj)
	{
		if (SourceObj)
		{
			uint32 UniqueID = SourceObj->GetUniqueID();
			if (Modifiers.Contains(UniqueID))
			{
				Modifiers.Remove(UniqueID);
				RecalculateValue();
			}
		}
	}

	bool HasCCImmunity(ECrowdControlEffect CCImmunity) const
	{
		return (Value & (1 << (uint8)CCImmunity));
	}

	bool HasCCImmunities(uint8 CCImmunities) const
	{
		return (Value & CCImmunities) == CCImmunities;
	}

private:

	float RecalculateValue()
	{
		Modifiers.ValueSort([&](const FCCImmunityModifier& Mod1, const FCCImmunityModifier& Mod2) { return Mod1 < Mod2; });

		Value = Value_NoMod;
		for (const TPair<uint32, FCCImmunityModifier>& ModPair : Modifiers)
		{
			if (ModPair.Value.ModType == ECCImmunityModType::Additive)
			{
				AddImmunities(ModPair.Value.Value);
			}
			else if (ModPair.Value.ModType == ECCImmunityModType::Reductive)
			{
				RemoveImmunities(ModPair.Value.Value);
			}
		}

		return Value;
	}

	void AddImmunity(ECrowdControlEffect CCImmunity)
	{
		Value |= (1 << (uint8)CCImmunity);
	}

	void AddImmunities(uint8 CCImmunities)
	{
		Value |= CCImmunities;
	}

	void RemoveImmunity(ECrowdControlEffect CCImmunity)
	{
		Value ^= (1 << (uint8)CCImmunity);
	}

	void RemoveImmunities(uint8 CCImmunities)
	{
		Value ^= CCImmunities;
	}

	void RemoveAllImmunities()
	{
		Value = 0;
	}

private:

	uint8 Value_NoMod;

	UPROPERTY()
	uint8 Value;

	UPROPERTY()
	TMap<uint32, FCCImmunityModifier> Modifiers;

};


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
	//  Health, Mana, and Stamina
	// --------------------------------------	

	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FPrimaryStat Health;

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FPrimaryStat Mana;
	
	UPROPERTY(ReplicatedUsing = OnRep_Stamina)
	FPrimaryStat Stamina;
	
	/** Percentage (0 to 1) of max health that will be considered low health */
	UPROPERTY(EditDefaultsOnly, Category = BaseStats)
	float LowHealthPercent;

	bool IsLowOnHealth();

	UFUNCTION()
	void OnHealthChanged(int32 MaxValue, int32 CurrentValue);

	UFUNCTION()
	void OnManaChanged(int32 MaxValue, int32 CurrentValue);

	UFUNCTION()
	void OnStaminaChanged(int32 MaxValue, int32 CurrentValue);


	// --------------------------------------
	//  Primary Stat Regeneration
	// --------------------------------------

	//~ @note If regeneration rates are not displayed to player in-game then they are not needed to be replicated
	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	FGenericStat HealthRegenRate;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	FGenericStat ManaRegenRate;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = BaseStats, AdvancedDisplay)
	FGenericStat StaminaRegenRate;
	
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

	FTimerHandle HealthRegenTimerHandle;
	FTimerHandle ManaRegenTimerHandle;
	FTimerHandle StaminaRegenTimerHandle;

	/** Starts health regeneration on player. Automatically stops once the health is full or if manually stopped */
	void ActivateHealthRegeneration();

	/** Starts health regeneration on player. Automatically stops once the mana is full or if manually stopped */
	void ActivateManaRegeneration();

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

	// --------------------------------------
	//  Attack
	// --------------------------------------

	UPROPERTY(Replicated)
	FGenericStat PhysicalAttack;

	UPROPERTY(Replicated)
	FGenericStat MagickalAttack;

	UPROPERTY(Replicated)
	FGenericStat PhysicalResistance;

	UPROPERTY(Replicated)
	FGenericStat MagickalResistance;

	UPROPERTY(Replicated)
	FGenericStat PhysicalCritRate;

	UPROPERTY(Replicated)
	FGenericStat MagickalCritRate;

	UPROPERTY(Replicated)
	FGenericStat PhysicalCritBonus;

	UPROPERTY(Replicated)
	FGenericStat MagickalCritBonus;

	//~ @todo future
	/*
	FSecondaryStat ElementalFireResistance;
	FSecondaryStat ElementalIceResistance;
	FSecondaryStat ElementalLightningResistance;
	FSecondaryStat ElementalPoisonResistance;
	FSecondaryStat ElementalHolyResistance;
	FSecondaryStat ElementalDarkResistance;

	FSecondaryStat ElementalFireDamage;
	FSecondaryStat ElementalIceDamage;
	FSecondaryStat ElementalLightningDamage;
	FSecondaryStat ElementalPoisonDamage;
	FSecondaryStat ElementalHolyDamage;
	FSecondaryStat ElementalDarkDamage;
	*/

	UPROPERTY(Replicated)
	FGenericStat BleedResistance;

	UPROPERTY(Replicated)
	FGenericStat CrowdControlResistance;

	UPROPERTY(Replicated)
	FGenericStat CooldownModifier;

	UPROPERTY(Replicated)
	FGenericStat ExpModifier;

	UPROPERTY(Replicated)
	FGenericStat SpellCastingSpeedModifier;

	UPROPERTY(Replicated)
	FGenericStat StaminaConsumptionModifier;

	//~ @todo future
	/*
	FStat_Float DropRateModifier;
	*/

	UPROPERTY(Replicated)
	FGenericStat PhysicalDamageReductionOnBlock;

	UPROPERTY(Replicated)
	FGenericStat MagickalDamageReductionOnBlock;

	UPROPERTY(Replicated)
	FCCImmunities CCImmunities;
	
	UPROPERTY(Replicated)
	FGenericStat Darkness;

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------
	
	UFUNCTION()
	virtual void OnRep_Health();
	
	UFUNCTION()
	virtual void OnRep_Mana();
	
	UFUNCTION()
	virtual void OnRep_Stamina();

};
