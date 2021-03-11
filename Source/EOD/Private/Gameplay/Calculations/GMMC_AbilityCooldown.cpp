// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Calculations/GMMC_AbilityCooldown.h"
#include "EODGameplayAbility.h"
#include "EODAbilitySystemComponent.h"
#include "Characters/EODCharacterBase.h"

float UGMMC_AbilityCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float CooldownVal = 0.f;
	if (Spec.GetContext().GetInstigatorAbilitySystemComponent())
	{
		AEODCharacterBase* Character = Cast<AEODCharacterBase>(Spec.GetContext().GetInstigatorAbilitySystemComponent()->GetOwnerActor());
		if (Character && Character->IsCooldownDisabled())
		{
			CooldownVal = 0.1f;
		}
	}

	const UEODGameplayAbility* Ability = Cast<UEODGameplayAbility>(Spec.GetContext().GetAbility());
	if (Ability)
	{
		CooldownVal = Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel()); 
	}
	
	return CooldownVal;
}
