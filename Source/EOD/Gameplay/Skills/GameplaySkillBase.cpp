// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameplaySkillBase.h"
#include "EODCharacterBase.h"
#include "Components/GameplaySkillsComponent.h"

UGameplaySkillBase::UGameplaySkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillGroup = NAME_None;
	CurrentUpgrade = 0;
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

void UGameplaySkillBase::ActivateSkill()
{
}

void UGameplaySkillBase::CancelSkill()
{
}

void UGameplaySkillBase::EndSkill()
{
}

bool UGameplaySkillBase::CanActivateSkill() const
{
	return false;
}

bool UGameplaySkillBase::CanCancelSkill() const
{
	return false;
}

void UGameplaySkillBase::OnOwnerWeaponChange(FName NewWeaponID, FWeaponTableRow* NewWeaponData, FName OldWeaponID, FWeaponTableRow* OldWeaponData)
{

}
