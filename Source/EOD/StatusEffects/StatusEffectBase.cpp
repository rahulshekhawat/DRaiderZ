// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffectBase.h"

#include "Engine/World.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UStatusEffectBase::CharacterToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();

UStatusEffectBase::UStatusEffectBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

void UStatusEffectBase::Initialize(AEODCharacterBase* Owner, AActor* Instigator)
{
	SetOwningCharacter(Owner);
	SetInstigator(Instigator);
}

void UStatusEffectBase::Deinitialize()
{
	for (TPair<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> CharacterToStatusInfoPair : CharacterToStatusInfoMap)
	{
		FStatusInfo& StatusInfo = CharacterToStatusInfoPair.Value;
		GetWorld()->GetTimerManager().ClearTimer(*StatusInfo.TimerHandle);
		delete StatusInfo.TimerHandle;
	}
	CharacterToStatusInfoMap.Empty();
}

void UStatusEffectBase::OnTriggerEvent(TArray<TWeakObjectPtr<AEODCharacterBase>>& RecipientCharacters)
{
	bool bShouldActivate = ActivationChance >= FMath::RandRange(0.f, 1.f) ? true : false;

	if (!bShouldActivate)
	{
		return;
	}

	for (TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter : RecipientCharacters)
	{
		ActivateStatusEffect(RecipientCharacter);
	}
}

void UStatusEffectBase::RequestDeactivation(AEODCharacterBase * Character)
{
}

void UStatusEffectBase::ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
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

void UStatusEffectBase::DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
}

AEODCharacterBase* UStatusEffectBase::GetOwningCharacter() const
{
	return OwningCharacter;
}

AActor* UStatusEffectBase::GetInstigator() const
{
	return Instigator;
}

void UStatusEffectBase::SetOwningCharacter(AEODCharacterBase* NewCharacter)
{
	OwningCharacter = NewCharacter;
}

void UStatusEffectBase::SetInstigator(AActor * NewInstigator)
{
	Instigator = NewInstigator;
}
