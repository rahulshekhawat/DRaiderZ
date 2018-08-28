// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DarkElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UDarkElemental::CursedCharactersToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();

UDarkElemental::UDarkElemental(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	bTriggersOnSuccessfulHit = true;
	ActivationChance = 0.1f;
	StackLimit = 1;

	// The status effect persists indefinitely
	StatusEffectDuration = -1;
	TickInterval = 1;
}

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* UDarkElemental::GetCharacterToStatusInfoMap()
{
	return &CursedCharactersToStatusInfoMap;
}

void UDarkElemental::OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper & WrappedRecipientCharacter)
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

	float OwnerDarkDamage = GetOwningCharacter()->StatsComp->GetElementalDarkDamage();
	float TargetDarkResistance = GetOwningCharacter()->StatsComp->GetElementalDarkResistance();

	float Damage = UCombatLibrary::CalculateDamage(OwnerDarkDamage, TargetDarkResistance);
	TargetCharacter->StatsComp->ModifyMaxHealth(-Damage);

}
