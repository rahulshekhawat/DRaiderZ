// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Abilities/EODGameplayAbility.h"
#include "Characters/EODCharacterBase.h"
#include "Gameplay/EODAbilitySystemComponent.h"

UEODGameplayAbility::UEODGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

	if (bActivatedAbility)
	{
	}
}
