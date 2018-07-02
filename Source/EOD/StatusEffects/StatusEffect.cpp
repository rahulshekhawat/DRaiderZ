// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
	bTriggersOnCritical = false;
	bTriggersOnReceivingHit = false;
	bTriggersOnSuccessfulDodge = false;
	bTriggersOnSuccessfulHit = false;
	bTriggersOnUnsuccessfulHit = false;
}

ABaseCharacter * UStatusEffect::GetOwningCharacter() const
{
	return OwningCharacter;
}

void UStatusEffect::SetOwningCharacter(ABaseCharacter * NewCharacter)
{
	OwningCharacter = NewCharacter;
}
