// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AISkillBase.h"
#include "AICharacterBase.h"
#include "GameplaySkillsComponent.h"

UAISkillBase::UAISkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSkillCanBeCharged = false;
	AnimationStartSectionName = FName("Default");
}

bool UAISkillBase::CanTriggerSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();

	if (Instigator)
	{
		bool bInstigatorCanUseSkill = Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking();
		return bInstigatorCanUseSkill && SkillMontage;
	}

	return false;
}

void UAISkillBase::TriggerSkill()
{
}

bool UAISkillBase::CanReleaseSkill() const
{
	return false;
}

void UAISkillBase::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}

	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if (SkillsComp)
	{
		SkillsComp->OnSkillFinished(SkillIndex, SkillGroup, this);
	}
}
