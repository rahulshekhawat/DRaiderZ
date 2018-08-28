// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HolyElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UHolyElemental::CharactersToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();
TMap<TWeakObjectPtr<AEODCharacterBase>, float> UHolyElemental::CharacterToDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();

UHolyElemental::UHolyElemental(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	bTriggersOnReceivingHit = true;
	ActivationChance = 0.2;
	StackLimit = 1;

	StatusEffectDuration = 10;
	TickInterval = StatusEffectDuration;

	// 10 percent
	MaxDefenseReduction = 0.1f;

}

void UHolyElemental::Deinitialize()
{
	Super::Deinitialize();
	CharacterToDefenseReductionMap.Empty();
}

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* UHolyElemental::GetCharacterToStatusInfoMap()
{
	return &CharactersToStatusInfoMap;
}

void UHolyElemental::OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper & WrappedRecipientCharacter)
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

	if (CharacterToDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) && StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		float DefenseReduction = CharacterToDefenseReductionMap[WrappedRecipientCharacter.RecipientCharacter];
		TargetCharacter->StatsComp->ModifyPhysicalResistance(DefenseReduction);
		TargetCharacter->StatsComp->ModifyMagickalResistance(DefenseReduction);
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
	}
	else if(!CharacterToDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter))
	{
		float OwnerHolyDamage = GetOwningCharacter()->StatsComp->GetElementalHolyDamage();
		float TargetHolyResistance = TargetCharacter->StatsComp->GetElementalHolyResistance();

		float DefenseReduction = CalculateDefenseReduction(OwnerHolyDamage, TargetHolyResistance);
		TargetCharacter->StatsComp->ModifyPhysicalResistance(-DefenseReduction);
		TargetCharacter->StatsComp->ModifyMagickalResistance(-DefenseReduction);
		CharacterToDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, DefenseReduction);
	}
}

void UHolyElemental::DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{
	Super::DeactivateStatusEffect(RecipientCharacter);
	CharacterToDefenseReductionMap.Remove(RecipientCharacter);
}

float UHolyElemental::CalculateDefenseReduction(float HolyDamage, float HolyDefense)
{
	return 0.0f;
}
