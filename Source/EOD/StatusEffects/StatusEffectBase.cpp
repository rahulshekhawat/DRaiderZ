// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffectBase.h"
#include "EOD/Player/EODCharacterBase.h"

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
	for (TPair<AEODCharacterBase*, FStatusInfo> CharacterToStatusInfoPair : CharacterToStatusInfoMap)
	{
		FStatusInfo& StatusInfo = CharacterToStatusInfoPair.Value;
		GetWorld()->GetTimerManager().ClearTimer(*StatusInfo.TimerHandlePtr.Get());
		StatusInfo.TimerHandlePtr.Reset();
	}
	CharacterToStatusInfoMap.Empty();
}

void UStatusEffectBase::OnTriggerEvent_Implementation(AEODCharacterBase* RecipientCharacter)
{
	bool bShouldActivate = ActivationChance >= FMath::RandRange(0.f, 1.f) ? true : false;
	if (bShouldActivate)
	{
		ActivateStatusEffect(RecipientCharacter);
	}
}

void UStatusEffectBase::RequestDeactivation(AEODCharacterBase* Character)
{
	DeactivateStatusEffect(Character);
}

void UStatusEffectBase::ActivateStatusEffect_Implementation(AEODCharacterBase* TargetCharacter)
{
	if (CharacterToStatusInfoMap.Contains(TargetCharacter))
	{
		if (!bResetsOnReactivation)
		{
			return;
		}

		FStatusInfo& StatusInfo = CharacterToStatusInfoMap[TargetCharacter];
		StatusInfo.CurrentStackLevel = StatusInfo.CurrentStackLevel >= StackLimit ? StackLimit : StatusInfo.CurrentStackLevel + 1;
		check(StatusInfo.TimerHandlePtr.IsValid());
		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandlePtr.Get(), StatusInfo.TimerDelegate, TickInterval, true, 0);
	}
	else
	{
		FStatusInfo StatusInfo;
		StatusInfo.CurrentStackLevel = 1;
		StatusInfo.TimerHandlePtr = MakeShareable(new FTimerHandle());
		StatusInfo.TimerDelegate.BindUFunction(this, FName("OnStatusEffectTick"), TargetCharacter);
		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandlePtr.Get(), StatusInfo.TimerDelegate, TickInterval, true, 0);
		CharacterToStatusInfoMap.Add(TargetCharacter, StatusInfo);
	}
}

void UStatusEffectBase::DeactivateStatusEffect_Implementation(AEODCharacterBase* TargetCharacter)
{
	FStatusInfo& StatusInfo = CharacterToStatusInfoMap[TargetCharacter];
	GetWorld()->GetTimerManager().ClearTimer(*StatusInfo.TimerHandlePtr.Get());
	StatusInfo.TimerHandlePtr.Reset();
	CharacterToStatusInfoMap.Remove(TargetCharacter);
}

void UStatusEffectBase::OnStatusEffectTick_Implementation(AEODCharacterBase* TargetCharacter)
{
}
