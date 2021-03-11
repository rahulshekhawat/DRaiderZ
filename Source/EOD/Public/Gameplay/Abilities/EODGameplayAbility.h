// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/EODTypes.h"
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

	inline EAbilityInputID GetAbilityInputID() const { return AbilityInputID; }

	inline bool GetActivateAbilityOnGranted() const { return bActivateAbilityOnGranted; }

	// UFUNCTION(BlueprintCallable, Category = "Raiderz Gameplay Ability")
	// bool 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FScalableFloat CooldownDuration;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	EAbilityInputID AbilityInputID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	bool bActivateAbilityOnGranted;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// If an ability is marked as 'bActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Raiderz Gameplay Ability")
	class AEODCharacterBase* OwningCharacter;

};
