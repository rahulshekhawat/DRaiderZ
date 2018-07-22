// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffect.h"

#include "Engine/World.h"

TMap<TWeakObjectPtr<ABaseCharacter>, FStatusTickInfo> UStatusEffect::CharacterToStatusTickInfoMap = TMap<TWeakObjectPtr<ABaseCharacter>, FStatusTickInfo>();

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

	// Default stack limit is 1
	StackLimit = 1;
	// Default activation chance is also 1
	ActivationChance = 1.f;
}

void UStatusEffect::Initialize(ABaseCharacter* Owner, AActor* Instigator)
{
	SetOwningCharacter(Owner);
	SetInstigator(Instigator);
}

void UStatusEffect::Deinitialize()
{
	for (TPair<TWeakObjectPtr<ABaseCharacter>, FStatusTickInfo> CharacterToStatusTickInfoPair : CharacterToStatusTickInfoMap)
	{
		FStatusTickInfo& StatusTickInfo = CharacterToStatusTickInfoPair.Value;
		GetWorld()->GetTimerManager().ClearTimer(*StatusTickInfo.TimerHandle);
		delete StatusTickInfo.TimerHandle;
	}
	CharacterToStatusTickInfoMap.Empty();
}

void UStatusEffect::OnTriggerEvent(TArray<TWeakObjectPtr<ABaseCharacter>>& RecipientCharacters)
{
	bool bShouldActivate = ActivationChance >= FMath::RandRange(0.f, 1.f) ? true : false;

	if (!bShouldActivate)
	{
		return;
	}

	for (TWeakObjectPtr<ABaseCharacter>& RecipientCharacter : RecipientCharacters)
	{
		ActivateStatusEffect(RecipientCharacter);
	}
}

void UStatusEffect::RequestDeactivation(ABaseCharacter * Character)
{
}

void UStatusEffect::ActivateStatusEffect(TWeakObjectPtr<ABaseCharacter>& RecipientCharacter)
{
	if (CharacterToStatusTickInfoMap.Contains(RecipientCharacter))
	{
		if (!bResetsOnReactivation)
		{
			return;
		}

		FStatusTickInfo& StatusTickInfo = CharacterToStatusTickInfoMap[RecipientCharacter];
		StatusTickInfo.CurrentStackLevel += 1;

		GetWorld()->GetTimerManager().SetTimer(*StatusTickInfo.TimerHandle, StatusTickInfo.TimerDelegate, TickInterval, true, 0);
	}
	else
	{
		FStatusTickInfo StatusTickInfo;
		StatusTickInfo.CurrentStackLevel = 1;
		StatusTickInfo.TimerHandle = new FTimerHandle;
		StatusTickInfo.TimerDelegate.BindUFunction(this, FName("OnStatusEffectTick"), StatusTickInfo);
		StatusTickInfo.RecipientCharacter = RecipientCharacter;

		GetWorld()->GetTimerManager().SetTimer(*StatusTickInfo.TimerHandle, StatusTickInfo.TimerDelegate, TickInterval, true, 0);

		CharacterToStatusTickInfoMap.Add(RecipientCharacter, StatusTickInfo);
	}
}

void UStatusEffect::DeactivateStatusEffect(TWeakObjectPtr<ABaseCharacter>& RecipientCharacter)
{
}

ABaseCharacter* UStatusEffect::GetOwningCharacter() const
{
	return OwningCharacter;
}

AActor* UStatusEffect::GetInstigator() const
{
	return Instigator;
}

void UStatusEffect::SetOwningCharacter(ABaseCharacter* NewCharacter)
{
	OwningCharacter = NewCharacter;
}

void UStatusEffect::SetInstigator(AActor * NewInstigator)
{
	Instigator = NewInstigator;
}
