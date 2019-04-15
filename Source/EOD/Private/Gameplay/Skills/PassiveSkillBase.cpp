// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PassiveSkillBase.h"
#include "GameplaySkillsComponent.h"

UPassiveSkillBase::UPassiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPassiveSkillBase::CancelSkill()
{
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillCancelled(SkillIndex, SkillGroup, this);
	}
}

bool UPassiveSkillBase::CanCancelSkill() const
{
	return false;
}
