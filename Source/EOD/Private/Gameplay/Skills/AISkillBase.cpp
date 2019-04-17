// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AISkillBase.h"

UAISkillBase::UAISkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSkillCanBeCharged = false;
}

bool UAISkillBase::CanReleaseSkill() const
{
	return false;
}
