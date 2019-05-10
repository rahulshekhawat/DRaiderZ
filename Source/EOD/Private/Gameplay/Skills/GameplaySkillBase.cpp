// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameplaySkillBase.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"

UGameplaySkillBase::UGameplaySkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillGroup = NAME_None;
	bSkillCanBeCharged = false;
	bNeedsUpdate = false;
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

void UGameplaySkillBase::CommitSkill()
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
	return bSkillCanBeCharged;
}

void UGameplaySkillBase::ReleaseSkill(float ChargeDuration)
{
}

void UGameplaySkillBase::UpdateSkill(float DeltaTime)
{
}

void UGameplaySkillBase::CancelSkill()
{
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillCancelled(SkillIndex, SkillGroup, this);
	}
}

void UGameplaySkillBase::FinishSkill()
{
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillFinished(SkillIndex, SkillGroup, this);
	}
}

void UGameplaySkillBase::TriggerGameplayEffects()
{
}

TSharedPtr<FAttackInfo> UGameplaySkillBase::GetAttackInfoPtr()
{
	return TSharedPtr<FAttackInfo>(nullptr);
}

bool UGameplaySkillBase::CanCancelSkill() const
{
	return true;
}
