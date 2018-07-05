// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
	bTriggersOnReceivingHit = false;
	bTriggersOnSuccessfulHit = false;
	bTriggersOnUnsuccessfulHit = false;
	bTriggersOnCriticalHit = false;
	bTriggersOnSuccessfulDodge = false;
	bTriggersOnSuccessfulBlock = false;
	bTriggersOnFullHealth = false;
	bTriggersOnLowHealth = false;
	bTriggersOnEnteringCombat = false;
	bTriggersOnLeavingCombat = false;
	bTriggersOnInitialization = false;
}

ABaseCharacter * UStatusEffect::GetOwningCharacter() const
{
	return OwningCharacter;
}

void UStatusEffect::SetOwningCharacter(ABaseCharacter * NewCharacter)
{
	OwningCharacter = NewCharacter;
}
