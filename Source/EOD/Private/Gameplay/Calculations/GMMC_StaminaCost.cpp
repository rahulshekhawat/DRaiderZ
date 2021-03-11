// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Calculations/GMMC_StaminaCost.h"
#include "EODGameplayAbility.h"

float UGMMC_StaminaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float StaminaCost = 0.f;
	const UEODGameplayAbility* Ability = Cast<UEODGameplayAbility>(Spec.GetContext().GetAbility());
	if (Ability)
	{
		StaminaCost = Ability->StaminaCost.GetValueAtLevel(Ability->GetAbilityLevel());
	}
	return StaminaCost;
}
