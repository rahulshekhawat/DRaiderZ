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
}

void UStatusEffect::Initialize(ABaseCharacter* Owner, AActor* Instigator)
{
	SetOwningCharacter(Owner);
	SetInstigator(Instigator);
}

void UStatusEffect::Deinitialize()
{
	// clean up all references
}

void UStatusEffect::OnTrigger(TArray<TWeakObjectPtr<ABaseCharacter>>& RecipientCharacters)
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

void UStatusEffect::ActivateStatusEffect(TWeakObjectPtr<ABaseCharacter>& RecipientCharacter)
{
	if (CharacterToStatusTickInfoMap.Contains(RecipientCharacter))
	{

	}
	else
	{
		FTimerHandle* TimerHandle = new FTimerHandle;
		FTimerDelegate TimerDelegate;
		
		FStatusTickInfo StatusTickInfo;
		StatusTickInfo.CurrentStackLevel = 1;
		StatusTickInfo.TimerHandle = TimerHandle;
		StatusTickInfo.RecipientCharacter = RecipientCharacter;

		TimerDelegate.BindUFunction(this, FName("OnStatusEffectTick"), StatusTickInfo);
		GetWorld()->GetTimerManager().SetTimer(*TimerHandle, TimerDelegate, TickInterval, true, 0);

		CharacterToStatusTickInfoMap.Add(RecipientCharacter, StatusTickInfo);
	}
}

void UStatusEffect::DeactivateStatusEffect()
{
}

void UStatusEffect::OnStatusEffectTick(FStatusTickInfo & StatusTickInfo)
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
