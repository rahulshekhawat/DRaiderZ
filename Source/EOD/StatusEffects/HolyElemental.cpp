// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HolyElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UHolyElemental::CharactersToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();
TMap<TWeakObjectPtr<AEODCharacterBase>, float> UHolyElemental::CharacterToPhysicalDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();
TMap<TWeakObjectPtr<AEODCharacterBase>, float> UHolyElemental::CharacterToMagickalDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();

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
	CharacterToPhysicalDefenseReductionMap.Empty();
	CharacterToMagickalDefenseReductionMap.Empty();
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

	if ((CharacterToMagickalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter)) &&
		StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
	}
	else if (CharacterToMagickalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter))
	{
		float OwnerHolyDamage = GetOwningCharacter()->StatsComp->GetElementalHolyDamage();
		float TargetHolyResistance = TargetCharacter->StatsComp->GetElementalHolyResistance();

		float PhysicalDefenseReduction = CalculatePhysicalDefenseReduction(OwnerHolyDamage, TargetHolyResistance);
		float MagickalDefenseReduction = CalculateMagickalDefenseReduction(OwnerHolyDamage, TargetHolyResistance);

		TargetCharacter->StatsComp->ModifyPhysicalResistance(-PhysicalDefenseReduction);
		TargetCharacter->StatsComp->ModifyMagickalResistance(-MagickalDefenseReduction);
		
		CharacterToPhysicalDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, PhysicalDefenseReduction);
		CharacterToMagickalDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, MagickalDefenseReduction);
	}
}

void UHolyElemental::DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{

	float PhysicalDefenseReduction = CharacterToPhysicalDefenseReductionMap[RecipientCharacter];
	float MagickalDefenseReduction = CharacterToMagickalDefenseReductionMap[RecipientCharacter];

	AEODCharacterBase* TargetCharacter = RecipientCharacter.Get();
	if (TargetCharacter)
	{
		TargetCharacter->StatsComp->ModifyPhysicalResistance(PhysicalDefenseReduction);
		TargetCharacter->StatsComp->ModifyMagickalResistance(MagickalDefenseReduction);
	}

	CharacterToPhysicalDefenseReductionMap.Remove(RecipientCharacter);
	CharacterToMagickalDefenseReductionMap.Remove(RecipientCharacter);

	Super::DeactivateStatusEffect(RecipientCharacter);
}

float UHolyElemental::CalculatePhysicalDefenseReduction(float HolyDamage, float HolyDefense)
{
	// @todo
	return 0.0f;
}

float UHolyElemental::CalculateMagickalDefenseReduction(float HolyDamage, float HolyDefense)
{
	// @todo
	return 0.0f;
}
