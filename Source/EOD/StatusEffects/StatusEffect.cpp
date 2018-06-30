// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
}

void UStatusEffect::OnSuccessfulHit(ABaseCharacter * HitCharacter)
{
}

void UStatusEffect::OnUnsuccessfulHit()
{
}

void UStatusEffect::OnActivation()
{
}

void UStatusEffect::OnDeactivation()
{
}

ABaseCharacter * UStatusEffect::GetOwningCharacter() const
{
	return OwningCharacter;
}

void UStatusEffect::SetOwningCharacter(ABaseCharacter * NewCharacter)
{
	OwningCharacter = NewCharacter;
}
