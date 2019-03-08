// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameplaySkillBase.h"
#include "EODCharacterBase.h"

UGameplaySkillBase::UGameplaySkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillGroup = NAME_None;
}

void UGameplaySkillBase::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	SkillInstigator = Instigator;
	SkillOwner = Owner;
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
