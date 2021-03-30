// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/EODTypes.h"
#include "AnimInstances/EODAction.h"
#include "Abilities/GameplayAbility.h"
#include "EODGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UEODGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UEODGameplayAbility(const FObjectInitializer& ObjectInitializer);
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	inline EAbilityInputID GetAbilityInputID() const { return AbilityInputID; }

	inline bool GetActivateAbilityOnGranted() const { return bActivateAbilityOnGranted; }

	UFUNCTION(BlueprintCallable, Category = "Raiderz Gameplay Ability")
	FEODAction GetActionForTag(const FGameplayTag& ActionTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FScalableFloat StaminaCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FScalableFloat ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FGameplayTagContainer CooldownTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FScalableFloat CooldownDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FGameplayTagContainer DamageDataTags;
	
	UFUNCTION(BlueprintPure, Category = "Raiderz Gameplay Ability")
	FGameplayEffectSpecHandle GenerateDamageSpec(float Modifier = 1.f) const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	EAbilityInputID AbilityInputID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	bool bActivateAbilityOnGranted;

	// If an ability is marked as 'bActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Raiderz Gameplay Ability")
	class AEODCharacterBase* OwningCharacter;

};
