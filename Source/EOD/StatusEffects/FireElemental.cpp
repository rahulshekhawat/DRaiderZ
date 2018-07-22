// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "FireElemental.h"
#include "Player/BaseCharacter.h"

UFireElemental::UFireElemental()
{
	bTriggersOnCriticalHit = true;
}

void UFireElemental::OnInitialize(ABaseCharacter* Owner, AActor* Instigator)
{
}

void UFireElemental::OnDeinitialize()
{
}

void UFireElemental::OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters)
{
	/*
	if (ReactivationCondition == EStatusEffectReactivationCondition::None)
	{
	}
	else if (ReactivationCondition == EStatusEffectReactivationCondition::Reset)
	{

	}
	else if (ReactivationCondition == EStatusEffectReactivationCondition::Stack)
	{

	}
	else
	{

	}
	*/
	/*
	for (ABaseCharacter* RecipientCharacter : RecipientCharacters)
	{
		RecipientCharacter->AddStatusEffectVisuals(Icon, ParticleSystem);
	}
	*/
}

void UFireElemental::OnDeactivation()
{
}
