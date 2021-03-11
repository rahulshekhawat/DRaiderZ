// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Calculations/GMMC_AbilityCooldown.h"
#include "EODGameplayAbility.h"
#include "EODAbilitySystemComponent.h"
#include "Characters/EODCharacterBase.h"

float UGMMC_AbilityCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (Spec.GetContext().GetInstigatorAbilitySystemComponent())
	{
		AEODCharacterBase* Character = Cast<AEODCharacterBase>(Spec.GetContext().GetInstigatorAbilitySystemComponent()->GetOwnerActor());
		if (Character && Character->IsCooldownDisabled())
		{
			return 0.1f;
		}
	}

	const UEODGameplayAbility* Ability = Cast<UEODGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());
	if (Ability)
	{
		return Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel()); 
	}
	
	return 0.f;
}
