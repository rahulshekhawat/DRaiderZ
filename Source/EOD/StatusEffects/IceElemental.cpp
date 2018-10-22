// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "IceElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UIceElemental::CharactersToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();
// TMap<TWeakObjectPtr<AEODCharacterBase>, float> UIceElemental::CharacterToPhysicalDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();
// TMap<TWeakObjectPtr<AEODCharacterBase>, float> UIceElemental::CharacterToMagickalDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();

UIceElemental::UIceElemental(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bResetsOnReactivation = true;

	SlowDownModifier = 0.2f;

	bTriggersOnSuccessfulHit = true;
	ActivationChance = 0.1;
	StackLimit = 1;

	StatusEffectDuration = 5.f;
	TickInterval = StatusEffectDuration;
}

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* UIceElemental::GetCharacterToStatusInfoMap()
{
	return &CharactersToStatusInfoMap;
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

	FStatusInfo& StatusInfo = (*GetCharacterToStatusInfoMap())[WrappedRecipientCharacter.RecipientCharacter];
	StatusInfo.TotalElapsedTime += GetWorld()->GetTimerManager().GetTimerElapsed(*StatusInfo.TimerHandle);

	/*
	if ((CharacterToMagickalDamageReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDamageReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter)) &&
		StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{

	}
	else if (CharacterToMagickalDamageReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDamageReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter))
	{

	}
	*/

	/*
	if (StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		TargetCharacter->GetStatsComponent()->ModifyActiveTimeDilation(SlowDownModifier);
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
		return;
	}
	if ((CharacterToMagickalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter)) &&
		StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		float PhysicalDefenseReduction = CharacterToPhysicalDefenseReductionMap[WrappedRecipientCharacter.RecipientCharacter];
		float MagickalDefenseReduction = CharacterToMagickalDefenseReductionMap[WrappedRecipientCharacter.RecipientCharacter];

		TargetCharacter->GetStatsComponent()->ModifyPhysicalResistance(PhysicalDefenseReduction);
		TargetCharacter->GetStatsComponent()->ModifyMagickalResistance(MagickalDefenseReduction);

		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
	}
	else if (CharacterToMagickalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter))
	{
		float OwnerHolyDamage = GetOwningCharacter()->GetStatsComponent()->GetElementalHolyDamage();
		float TargetHolyResistance = TargetCharacter->GetStatsComponent()->GetElementalHolyResistance();

		float PhysicalDefenseReduction = CalculatePhysicalDefenseReduction(OwnerHolyDamage, TargetHolyResistance);
		float MagickalDefenseReduction = CalculateMagickalDefenseReduction(OwnerHolyDamage, TargetHolyResistance);

		TargetCharacter->GetStatsComponent()->ModifyPhysicalResistance(-PhysicalDefenseReduction);
		TargetCharacter->GetStatsComponent()->ModifyMagickalResistance(-MagickalDefenseReduction);

		CharacterToPhysicalDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, PhysicalDefenseReduction);
		CharacterToMagickalDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, MagickalDefenseReduction);
	}
	*/

	// @todo
	// Character->GetStatsComponent()->ModifyActiveTimeDilation()

}

float UIceElemental::CalculatePhysicalDamageReduction(float IceDamage, float IceDefense)
{
	// @todo
	return 0.0f;
}

float UIceElemental::CalculateMagickalDamageReduction(float IceDamage, float IceDefense)
{
	// @todo
	return 0.0f;
}
