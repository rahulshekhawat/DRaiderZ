// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InstantMeleeSkill.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"
#include "EODCharacterMovementComponent.h"

UInstantMeleeSkill::UInstantMeleeSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInstantMeleeSkill::TriggerSkill()
{
	Super::TriggerSkill();
}

bool UInstantMeleeSkill::CanCancelSkill() const
{
	return true;
}

void UInstantMeleeSkill::CancelSkill()
{
	Super::CancelSkill();
}

void UInstantMeleeSkill::FinishSkill()
{
	Super::FinishSkill();
}
