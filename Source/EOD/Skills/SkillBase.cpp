// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBase.h"

USkillBase::USkillBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void USkillBase::NativeOnUse()
{
	OnUse();
}
