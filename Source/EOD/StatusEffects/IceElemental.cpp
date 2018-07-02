// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "IceElemental.h"

UIceElemental::UIceElemental()
{
	bTriggersOnSuccessfulHit = true;
}

void UIceElemental::OnReceivingHit(ABaseCharacter * HittingCharacter)
{
}

void UIceElemental::OnSuccessfulHit(ABaseCharacter * HitCharacter)
{

}

void UIceElemental::OnUnsuccessfulHit()
{
}

void UIceElemental::OnInitialize(ABaseCharacter * Owner)
{
	// Owner is needed to activate buffs no self or allies
	SetOwningCharacter(Owner);
	
}

void UIceElemental::OnDeinitialize()
{
	SetOwningCharacter(nullptr);
	// @todo remove any timers that this status effect has activated on enemies?
	// @todo maybe call ConditionalBeginDestroy() on this object?
	// this->ConditionalBeginDestroy();
}

void UIceElemental::OnActivation()
{
}

void UIceElemental::OnDeactivation()
{
}
