// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffectBase.h"
#include "Player/EODCharacterBase.h"

#include "Engine/World.h"

// TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UStatusEffectBase::CharacterToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();

UStatusEffectBase::UStatusEffectBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTriggersOnReceivingHit				= false;
	bTriggersOnSuccessfulHit			= false;
	bTriggersOnSuccessfulPhysicalAttack = false;
	bTriggersOnSuccessfulMagickAttack	= false;
	bTriggersOnKillingEnemy				= false;
	// bTriggersOnUnsuccessfulHit			= false;
	bTriggersOnCriticalHit				= false;
	bTriggersOnSuccessfulDodge			= false;
	bTriggersOnSuccessfulBlock			= false;
	bTriggersOnFullHealth				= false;
	bTriggersOnLowHealth				= false;
	bTriggersOnEnteringCombat			= false;
	bTriggersOnLeavingCombat			= false;
	bTriggersOnInitialization			= false;
	bTriggersOnUsingSkill				= false;
	bTriggersOnSuccessfulSkillAttack	= false;

	// Default stack limit is 1
	StackLimit = 1;
	// Default activation chance is also 1
	ActivationChance = 1.f;
}

void UStatusEffectBase::Initialize(AEODCharacterBase* NewOwner, AActor* NewInstigator)
{
	SetOwningCharacter(NewOwner);
	SetInstigator(NewInstigator);
}

void UStatusEffectBase::Deinitialize()
{
	for (TPair<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> CharacterToStatusInfoPair : *GetCharacterToStatusInfoMap())
	{
		FStatusInfo& StatusInfo = CharacterToStatusInfoPair.Value;
		GetWorld()->GetTimerManager().ClearTimer(*StatusInfo.TimerHandle);
		delete StatusInfo.TimerHandle;
		StatusInfo.TimerHandle = nullptr;
	}

	GetCharacterToStatusInfoMap()->Empty();
}

void UStatusEffectBase::OnTriggerEvent_Implementation(AEODCharacterBase * RecipientCharacter)
{
}

void UStatusEffectBase::RequestDeactivation(AEODCharacterBase* Character)
{
	// TWeakObjectPtr<AEODCharacterBase> RecipientCharacter(Character);
	// DeactivateStatusEffect(RecipientCharacter);
}

void UStatusEffectBase::ActivateStatusEffect_Implementation(AEODCharacterBase * TargetCharacter)
{
}

void UStatusEffectBase::DeactivateStatusEffect_Implementation(AEODCharacterBase* TargetCharacter)
{
}

void UStatusEffectBase::OnStatusEffectTick_Implementation(AEODCharacterBase* TargetCharacter)
{
}

/*
void UStatusEffectBase::OnTriggerEvent(AEODCharacterBase* RecipientCharacter)
{
	bool bShouldActivate = ActivationChance >= FMath::RandRange(0.f, 1.f) ? true : false;
	if (bShouldActivate && RecipientCharacter)
	{
		ActivateStatusEffect(RecipientCharacter);
	}
}
*/



/*
void UStatusEffectBase::OnTriggerEvent(TArray<TWeakObjectPtr<AEODCharacterBase>>& RecipientCharacters)
{
	for (TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter : RecipientCharacters)
	{
		bool bShouldActivate = ActivationChance >= FMath::RandRange(0.f, 1.f) ? true : false;

		if (!bShouldActivate)
		{
			continue;
		}

		ActivateStatusEffect(RecipientCharacter);
	}
}
*/

/*
void UStatusEffectBase::ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
	if (GetCharacterToStatusInfoMap()->Contains(RecipientCharacter))
	{
		if (!bResetsOnReactivation)
		{
			return;
		}

		FStatusInfo& StatusInfo = (*GetCharacterToStatusInfoMap())[RecipientCharacter];
		StatusInfo.CurrentStackLevel = StatusInfo.CurrentStackLevel >= StackLimit ? StackLimit : StatusInfo.CurrentStackLevel + 1;
		// StatusInfo.CurrentStackLevel += 1;

		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandle, StatusInfo.TimerDelegate, TickInterval, true, 0);
	}
	else
	{
		FStatusInfo StatusInfo;
		StatusInfo.CurrentStackLevel = 1;
		StatusInfo.TimerHandle = new FTimerHandle;
		StatusInfo.TimerDelegate.BindUFunction(this, FName("OnStatusEffectTick"), StatusInfo);

		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandle, StatusInfo.TimerDelegate, TickInterval, true, 0);

		GetCharacterToStatusInfoMap()->Add(RecipientCharacter, StatusInfo);
	}
}

void UStatusEffectBase::DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
	FStatusInfo& StatusInfo = (*GetCharacterToStatusInfoMap())[RecipientCharacter];
	GetWorld()->GetTimerManager().ClearTimer(*StatusInfo.TimerHandle);
	delete StatusInfo.TimerHandle;
	StatusInfo.TimerHandle = nullptr;
	GetCharacterToStatusInfoMap()->Remove(RecipientCharacter);
}
*/
