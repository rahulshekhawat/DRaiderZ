// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PassiveSkillBase.h"

UPassiveSkillBase::UPassiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPassiveSkillBase::ActivateSkill()
{
}

void UPassiveSkillBase::CancelSkill()
{
}

void UPassiveSkillBase::EndSkill()
{
}

bool UPassiveSkillBase::CanActivateSkill() const
{
	return false;
}

bool UPassiveSkillBase::CanCancelSkill() const
{
	return false;
}
