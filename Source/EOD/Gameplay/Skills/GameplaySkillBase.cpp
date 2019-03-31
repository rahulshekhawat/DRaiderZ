// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameplaySkillBase.h"
#include "EODCharacterBase.h"
#include "Components/GameplaySkillsComponent.h"

UGameplaySkillBase::UGameplaySkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillGroup = NAME_None;
	CurrentUpgrade = 0;
	MaxUpgrades = 1;
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

void UGameplaySkillBase::Reinitialize()
{
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
	return true;
}

void UGameplaySkillBase::ReleaseSkill(float ChargeDuration)
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
