// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Calculations/GMMC_HealthRegen.h"
#include "CharacterAttributeSetBase.h"
#include "EODCharacterBase.h"

float UGMMC_HealthRegen::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float RegenValue = 0.f;
	AEODCharacterBase* InstigatorChar = Cast<AEODCharacterBase>(Spec.GetContext().GetInstigator());
	if (InstigatorChar && InstigatorChar->GetPrimaryAttributeSet())
	{
		RegenValue = InstigatorChar->GetPrimaryAttributeSet()->GetHealthRegenRate();
	}
	return RegenValue;
}
