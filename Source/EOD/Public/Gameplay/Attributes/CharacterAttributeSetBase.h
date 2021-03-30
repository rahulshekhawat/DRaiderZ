// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h These macros can be used to identify Attributes and get / set their values
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class EOD_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UCharacterAttributeSetBase();

	/**
	 *	Called just before any modification happens to an attribute. This is lower level than PreAttributeModify/PostAttribute modify.
	 *	There is no additional context provided here since anything can trigger this. Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	 *	This function is meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	 *
	 *	NewValue is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 *	Called just before modifying the value of an attribute. AttributeSet can make additional modifications here. Return true to continue, or false to throw out the modification.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 second +10 movement speed buff.
	 */
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	/**
	 *	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 second +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:

	//~
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)
	
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)
	
	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Mana)
	
	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxMana)
	
	UPROPERTY(ReplicatedUsing = OnRep_Stamina, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Stamina)
	
	UPROPERTY(ReplicatedUsing = OnRep_MaxStamina, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxStamina)
	
	UPROPERTY(ReplicatedUsing = OnRep_HealthRegenRate, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, HealthRegenRate)
	
	UPROPERTY(ReplicatedUsing = OnRep_ManaRegenRate, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, ManaRegenRate)
	
	UPROPERTY(ReplicatedUsing = OnRep_StaminaRegenRate, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, StaminaRegenRate)
	//~
	
	//~
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalAttack, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, PhysicalAttack)
	
	UPROPERTY(ReplicatedUsing = OnRep_MagicAttack, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicAttack;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MagicAttack)
	
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, PhysicalResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_MagicResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MagicResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalCritRate, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalCritRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, PhysicalCritRate)
	
	UPROPERTY(ReplicatedUsing = OnRep_MagicCritRate, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicCritRate;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MagicCritRate)
	
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalCritBonus, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PhysicalCritBonus;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, PhysicalCritBonus)
	
	UPROPERTY(ReplicatedUsing = OnRep_MagicCritBonus, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MagicCritBonus;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MagicCritBonus)
	//~
	

	//~ Begin Elemental Attributes
	UPROPERTY(ReplicatedUsing = OnRep_FireDamage, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData FireDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, FireDamage)
	
	UPROPERTY(ReplicatedUsing = OnRep_IceDamage, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData IceDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, IceDamage)
	
	UPROPERTY(ReplicatedUsing = OnRep_LightningDamage, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData LightningDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, LightningDamage)
	
	UPROPERTY(ReplicatedUsing = OnRep_PoisonDamage, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PoisonDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, PoisonDamage)
	
	UPROPERTY(ReplicatedUsing = OnRep_HolyDamage, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HolyDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, HolyDamage)
	
	UPROPERTY(ReplicatedUsing = OnRep_DarkDamage, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DarkDamage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, DarkDamage)
	
	UPROPERTY(ReplicatedUsing = OnRep_FireResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, FireResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_IceResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, IceResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_LightningResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, LightningResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_PoisonResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData PoisonResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, PoisonResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_HolyResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData HolyResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, HolyResistance)
	
	UPROPERTY(ReplicatedUsing = OnRep_DarkResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DarkResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, DarkResistance)
	//~ End Elemental Attributes

	
	UPROPERTY(ReplicatedUsing = OnRep_BleedResistance, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData BleedResistance;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, BleedResistance)

	
	// Physical Damage is a meta attribute used by the PhysicalDamageExecution to calculate final damage, which then turns into -Health
    // Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Damage)

protected:

	//~ Begin Rep Notifies
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_ManaRegenRate(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_PhysicalAttack(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MagicAttack(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MagicResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_PhysicalCritRate(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MagicCritRate(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_PhysicalCritBonus(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MagicCritBonus(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_FireDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_IceDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_LightningDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_PoisonDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_HolyDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_DarkDamage(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_FireResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_IceResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_LightningResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_PoisonResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_HolyResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_DarkResistance(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_BleedResistance(const FGameplayAttributeData& OldValue);
	//~ End Rep Notifies

protected:
	
	// Called when a "Max" attribute is changed to maintain the current percentage of min / max values
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;
	
	
};
