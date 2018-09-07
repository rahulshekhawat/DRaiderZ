// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HolyElemental.h"
#include "Player/EODCharacterBase.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> UHolyElemental::CharactersToStatusInfoMap = TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>();
TMap<TWeakObjectPtr<AEODCharacterBase>, float> UHolyElemental::CharacterToPhysicalDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();
TMap<TWeakObjectPtr<AEODCharacterBase>, float> UHolyElemental::CharacterToMagickDefenseReductionMap = TMap<TWeakObjectPtr<AEODCharacterBase>, float>();

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
	CharacterToMagickDefenseReductionMap.Empty();
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

	if ((CharacterToMagickDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter)) &&
		StatusInfo.TotalElapsedTime >= StatusEffectDuration)
	{
		DeactivateStatusEffect(WrappedRecipientCharacter.RecipientCharacter);
	}
	else if (CharacterToMagickDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter) &&
		CharacterToPhysicalDefenseReductionMap.Contains(WrappedRecipientCharacter.RecipientCharacter))
	{
		float OwnerHolyDamage = GetOwningCharacter()->StatsComp->GetElementalHolyDamage();
		float TargetHolyResistance = TargetCharacter->StatsComp->GetElementalHolyResistance();

		float PhysicalDefenseReduction = CalculatePhysicalDefenseReduction(OwnerHolyDamage, TargetHolyResistance);
		float MagickDefenseReduction = CalculateMagickDefenseReduction(OwnerHolyDamage, TargetHolyResistance);

		TargetCharacter->StatsComp->ModifyPhysicalResistance(-PhysicalDefenseReduction);
		// TargetCharacter->StatsComp->ModifyMagickalResistance(-MagickalDefenseReduction);
		TargetCharacter->StatsComp->ModifyMagickResistance(-MagickDefenseReduction);
		
		CharacterToPhysicalDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, PhysicalDefenseReduction);
		CharacterToMagickDefenseReductionMap.Add(WrappedRecipientCharacter.RecipientCharacter, MagickDefenseReduction);
	}
}

void UHolyElemental::DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter)
{

	float PhysicalDefenseReduction = CharacterToPhysicalDefenseReductionMap[RecipientCharacter];
	float MagickDefenseReduction = CharacterToMagickDefenseReductionMap[RecipientCharacter];

	AEODCharacterBase* TargetCharacter = RecipientCharacter.Get();
	if (TargetCharacter)
	{
		TargetCharacter->StatsComp->ModifyPhysicalResistance(PhysicalDefenseReduction);
		TargetCharacter->StatsComp->ModifyMagickResistance(MagickDefenseReduction);
	}

	CharacterToPhysicalDefenseReductionMap.Remove(RecipientCharacter);
	CharacterToMagickDefenseReductionMap.Remove(RecipientCharacter);

	Super::DeactivateStatusEffect(RecipientCharacter);
}

float UHolyElemental::CalculatePhysicalDefenseReduction(float HolyDamage, float HolyDefense)
{
	// @todo
	return 0.0f;
}

float UHolyElemental::CalculateMagickDefenseReduction(float HolyDamage, float HolyDefense)
{
	// @todo
	return 0.0f;
}
