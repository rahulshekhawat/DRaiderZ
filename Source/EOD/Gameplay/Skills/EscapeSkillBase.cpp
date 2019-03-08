// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EscapeSkillBase.h"
#include "EODCharacterBase.h"

UEscapeSkillBase::UEscapeSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEscapeSkillBase::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	Super::InitSkill(Instigator, Owner);
}

void UEscapeSkillBase::ActivateSkill()
{
}

void UEscapeSkillBase::CancelSkill()
{
}

void UEscapeSkillBase::EndSkill()
{
}
