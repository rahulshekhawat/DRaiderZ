// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ActiveSkillBase.h"
#include "EODCharacterBase.h"

UActiveSkillBase::UActiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupersedingSkillGroup = NAME_None;
}

void UActiveSkillBase::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	Super::InitSkill(Instigator, Owner);

	if (Instigator)
	{
		if (Instigator->Gender == ECharacterGender::Male)
		{

		}
		else
		{

		}
	}
}

void UActiveSkillBase::Reinitialize()
{
}

void UActiveSkillBase::ActivateSkill()
{

}

void UActiveSkillBase::CancelSkill()
{
}

void UActiveSkillBase::EndSkill()
{
}

bool UActiveSkillBase::CanActivateSkill() const
{
	return false;
}

bool UActiveSkillBase::CanCancelSkill() const
{
	return false;
}
