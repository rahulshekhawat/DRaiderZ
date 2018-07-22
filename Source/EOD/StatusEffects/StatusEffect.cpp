// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffect.h"

#include "Engine/World.h"

TMap<TWeakObjectPtr<ABaseCharacter>, FStatusInfo> UStatusEffect::CharacterToStatusInfoMap = TMap<TWeakObjectPtr<ABaseCharacter>, FStatusInfo>();

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
	for (TPair<TWeakObjectPtr<ABaseCharacter>, FStatusInfo> CharacterToStatusInfoPair : CharacterToStatusInfoMap)
	{
		FStatusInfo& StatusInfo = CharacterToStatusInfoPair.Value;
		GetWorld()->GetTimerManager().ClearTimer(*StatusInfo.TimerHandle);
		delete StatusInfo.TimerHandle;
	}
	CharacterToStatusInfoMap.Empty();
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
	if (CharacterToStatusInfoMap.Contains(RecipientCharacter))
	{
		if (!bResetsOnReactivation)
		{
			return;
		}

		FStatusInfo& StatusInfo = CharacterToStatusInfoMap[RecipientCharacter];
		StatusInfo.CurrentStackLevel += 1;

		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandle, StatusInfo.TimerDelegate, TickInterval, true, 0);
	}
	else
	{
		FStatusInfo StatusInfo;
		StatusInfo.CurrentStackLevel = 1;
		StatusInfo.TimerHandle = new FTimerHandle;
		StatusInfo.TimerDelegate.BindUFunction(this, FName("OnStatusEffectTick"), StatusInfo);
		// StatusInfo.RecipientCharacter = RecipientCharacter;

		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandle, StatusInfo.TimerDelegate, TickInterval, true, 0);

		CharacterToStatusInfoMap.Add(RecipientCharacter, StatusInfo);
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
