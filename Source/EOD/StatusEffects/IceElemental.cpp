// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "IceElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"


UIceElemental::UIceElemental(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StackLimit = 1;
	StatusEffectDuration = 10.f;
	ActivationChance = 0.1f;
	bResetsOnReactivation = true;
	SlowDownModifier = 0.2f;
}

void UIceElemental::ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
	// @todo

	if (CharacterToStatusInfoMap.Contains(RecipientCharacter))
	{
		if (!bResetsOnReactivation)
		{
		}
	}

	/*
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

		GetWorld()->GetTimerManager().SetTimer(*StatusInfo.TimerHandle, StatusInfo.TimerDelegate, TickInterval, true, 0);

		CharacterToStatusInfoMap.Add(RecipientCharacter, StatusInfo);
	}
	*/
}

void UIceElemental::OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter)
{
	AEODCharacterBase* TargetCharacter = WrappedRecipientCharacter.RecipientCharacter.Get();

	// In case the target character has been destroyed (dead)
	if (!IsValid(TargetCharacter))
	{
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
		return;
	}

	FStatusInfo& StatusInfo = CharacterToStatusInfoMap[WrappedRecipientCharacter.RecipientCharacter];
	StatusInfo.TotalElapsedTime += GetWorld()->GetTimerManager().GetTimerElapsed(*StatusInfo.TimerHandle);


	if (StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		TargetCharacter->StatsComp->ModifyActiveTimeDilation(SlowDownModifier);
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
		return;
	}

	// @todo
	// Character->StatsComp->ModifyActiveTimeDilation()

}
