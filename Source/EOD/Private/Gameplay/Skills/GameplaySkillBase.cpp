// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameplaySkillBase.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"

UGameplaySkillBase::UGameplaySkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillGroup = NAME_None;
	bSkillCanBeCharged = false;
}

void UGameplaySkillBase::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	SkillInstigator = Instigator;
	SkillOwner = Owner;
	if (Instigator)
	{
		InstigatorSkillComponent = Instigator->GetGameplaySkillsComponent();
	}
}

bool UGameplaySkillBase::CanTriggerSkill() const
{
	return true;
}

void UGameplaySkillBase::TriggerSkill()
{
}

bool UGameplaySkillBase::CanReleaseSkill() const
{
	return bSkillCanBeCharged;
}

void UGameplaySkillBase::ReleaseSkill(float ChargeDuration)
{
}

void UGameplaySkillBase::UpdateSkill(float DeltaTime)
{
}

void UGameplaySkillBase::CancelSkill()
{
}

void UGameplaySkillBase::FinishSkill()
{
}

bool UGameplaySkillBase::CanCancelSkill() const
{
	return true;
}

void UGameplaySkillBase::OnOwnerWeaponChange(FName NewWeaponID, FWeaponTableRow* NewWeaponData, FName OldWeaponID, FWeaponTableRow* OldWeaponData)
{

}
