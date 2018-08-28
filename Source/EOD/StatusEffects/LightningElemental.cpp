// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "LightningElemental.h"
#include "Player/EODCharacterBase.h"

ULightningElemental::ULightningElemental(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	bTriggersOnSuccessfulHit = true;
		
	ActivationChance = 0.1f;
	StunDuration = 1.f;
}

void ULightningElemental::Deinitialize()
{
	// empty override is intentional
}

void ULightningElemental::ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
	AEODCharacterBase* TargetCharacter = RecipientCharacter.Get();

	if (!TargetCharacter)
	{
		return;
	}

	if (TargetCharacter->CanBeStunned())
	{
		TargetCharacter->ApplyStun(StunDuration);
	}
}

void ULightningElemental::DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
	// empty override is intentional
}
