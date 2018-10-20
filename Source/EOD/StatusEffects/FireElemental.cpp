// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "FireElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UFireElemental::BurningCharactersToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();

UFireElemental::UFireElemental(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	bTriggersOnCriticalHit = true;
	ActivationChance = 0.5;
	StackLimit = 3;

	StatusEffectDuration = 10;
	TickInterval = 1;
}

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* UFireElemental::GetCharacterToStatusInfoMap()
{
	return &BurningCharactersToStatusInfoMap;
}

void UFireElemental::OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper & WrappedRecipientCharacter)
{
	AEODCharacterBase* TargetCharacter = WrappedRecipientCharacter.RecipientCharacter.Get();

	// In case the target character has been destroyed (dead)
	if (!IsValid(TargetCharacter))
	{
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
		return;
	}

	FStatusInfo& StatusInfo = (*GetCharacterToStatusInfoMap())[WrappedRecipientCharacter.RecipientCharacter];
	StatusInfo.TotalElapsedTime += GetWorld()->GetTimerManager().GetTimerElapsed(*StatusInfo.TimerHandle);

	float OwnerFireDamage = GetOwningCharacter()->GetStatsComponent()->GetElementalFireDamage();
	float TargetFireResistance = TargetCharacter->GetStatsComponent()->GetElementalFireResistance();

	float Damage = UCombatLibrary::CalculateDamage(OwnerFireDamage, TargetFireResistance);
	TargetCharacter->GetStatsComponent()->ModifyMaxHealth(-Damage);

	if (StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
		return;
	}
}
