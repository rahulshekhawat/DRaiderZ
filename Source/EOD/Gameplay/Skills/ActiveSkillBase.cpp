// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ActiveSkillBase.h"

UActiveSkillBase::UActiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupersedingSkillGroup = NAME_None;
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
