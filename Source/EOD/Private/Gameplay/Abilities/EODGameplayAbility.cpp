// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Abilities/EODGameplayAbility.h"
#include "Calculations/GMMC_AbilityCooldown.h"
#include "Characters/EODCharacterBase.h"
#include "Gameplay/EODAbilitySystemComponent.h"

UEODGameplayAbility::UEODGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UEODGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bCanActivate && CurrentActorInfo)
	{
		FGameplayAbilitySpec* Spec = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(Handle);
		if (Spec)
		{
			UEODGameplayAbility* AbilityInstance = Cast<UEODGameplayAbility>(Spec->GetPrimaryInstance());
			if (AbilityInstance)
			{
				if (AbilityInstance->K2_CheckAbilityCooldown() == false)
				{
					bCanActivate = false;
				}
				else if (AbilityInstance->K2_CheckAbilityCost() == false)
				{
					bCanActivate = false;
				}
			}
		}
	}
	return bCanActivate;
}

FEODAction UEODGameplayAbility::GetActionForTag(const FGameplayTag& ActionTag)
{
	return FEODAction();
}

void UEODGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);	
}

void UEODGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	if (OwningCharacter == nullptr)
	{
		if (CurrentActorInfo)
		{
			OwningCharacter = Cast<AEODCharacterBase>(GetAvatarActorFromActorInfo());
		}
	}

	bool bActivatedAbility = false;
	if (bActivateAbilityOnGranted && ActorInfo && ActorInfo->AbilitySystemComponent != nullptr)
	{
		bActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
