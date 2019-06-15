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

/**
 * Delegate for when the CC Immunity changes
 * @param1 uint8 NewImmunities
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCCImmunitiesChangedMCDelegate, uint8);


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

	FStatModifier(float InValue, EStatModType InModType, int32 InModOrder) :
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
		bDirty(false),
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
		MaxValue_NoMod = InValue <= 0 ? 1 : InValue;
		bDirty = true;
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
		CurrentValue = Max;
		OnStatValueChanged.Broadcast(Max, CurrentValue);
	}

	void SetCurrentValue(int32 InValue)
	{
		int32 Max = GetMaxValue();
		CurrentValue = InValue <= 0 ? 0 : InValue >= Max ? Max : InValue;
		OnStatValueChanged.Broadcast(Max, CurrentValue);
	}

	int32 RecalculateMaxValue()
	{
		Modifiers.ValueSort([&](const FStatModifier& Mod1, const FStatModifier& Mod2) { return Mod1 < Mod2; });

		MaxValue = MaxValue_NoMod;
		for (const TPair<uint32, FStatModifier>& ModPair : Modifiers)
		{
			if (ModPair.Value.ModType == EStatModType::Flat)
			{
				MaxValue += ModPair.Value.Value;
			}
			else if (ModPair.Value.ModType == EStatModType::Percent)
			{
				MaxValue += (MaxValue * (ModPair.Value.Value / 100.f));
			}
		}

		OnStatValueChanged.Broadcast(MaxValue, CurrentValue);

		bDirty = false;
		return MaxValue;
	}

	int32 GetMaxValue() { return bDirty ? RecalculateMaxValue() : MaxValue; }

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
			bDirty = true;
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
				bDirty = true;
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

	bool bDirty;
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
		bDirty(false),
		Value_NoMod(0),
		Value(0)
	{
	}

	FGenericStat(float InValue)
	{
		SetValue(InValue);
		RecalculateValue();
	}

public:

	/** Directly set the value that doesn't have any modifier applied */
	void SetValue(float InValue)
	{
		Value_NoMod = InValue <= 0 ? 1 : InValue;
		bDirty = true;
	}

	float RecalculateValue()
	{
		Modifiers.ValueSort([&](const FStatModifier& Mod1, const FStatModifier& Mod2) { return Mod1 < Mod2; });

		Value = Value_NoMod;
		for (const TPair<uint32, FStatModifier>& ModPair : Modifiers)
		{
			if (ModPair.Value.ModType == EStatModType::Flat)
			{
				Value += ModPair.Value.Value;
			}
			else if (ModPair.Value.ModType == EStatModType::Percent)
			{
				Value += (Value * (ModPair.Value.Value / 100.f));
			}
		}

		OnStatValueChanged.Broadcast(Value);

		bDirty = false;
		return Value;
	}

	float GetValue() { return bDirty ? RecalculateValue() : Value; }

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
			bDirty = true;
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
				bDirty = true;
			}
		}
	}

	FOnGenericStatChangedMCDelegate OnStatValueChanged;

	void ForceBroadcastDelegate()
	{
		float CV = GetValue();
		OnStatValueChanged.Broadcast(CV);
	}

private:

	bool bDirty;
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

	FCCImmunityModifier(uint8 InValue, ECCImmunityModType InModType, int32 InModOrder) :
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
		bDirty(false),
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
		bDirty = true;
	}

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

		bDirty = false;
		return Value;
	}

	float GetValue() { return bDirty ? RecalculateValue() : Value; }

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
			bDirty = true;
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
				bDirty = true;
			}
		}
	}

	FOnCCImmunitiesChangedMCDelegate OnStatValueChanged;

	void ForceBroadcastDelegate()
	{
		uint8 CV = GetValue();
		OnStatValueChanged.Broadcast(CV);
	}

private:

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

	bool HasCCImmunity(ECrowdControlEffect CCImmunity)
	{
		return (Value & (1 << (uint8)CCImmunity));
	}

private:

	bool bDirty;
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
