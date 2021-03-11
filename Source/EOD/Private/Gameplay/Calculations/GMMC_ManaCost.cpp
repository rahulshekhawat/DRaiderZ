// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Calculations/GMMC_ManaCost.h"
#include "EODGameplayAbility.h"

float UGMMC_ManaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float ManaCost = 0.f;
	const UEODGameplayAbility* Ability = Cast<UEODGameplayAbility>(Spec.GetContext().GetAbility());
	if (Ability)
	{
		ManaCost = Ability->ManaCost.GetValueAtLevel(Ability->GetAbilityLevel());
	}
	return ManaCost;
}
