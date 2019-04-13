// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "PlayerSkillBase.h"

UPlayerSkillBase::UPlayerSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentUpgrade = 0;
	MaxUpgrades = 1;
}

void UPlayerSkillBase::OnWeaponChange(EWeaponType NewWeaponType, EWeaponType OldWeaponType)
{
}

void UPlayerSkillBase::OnActivatedAsChainSkill()
{
}

void UPlayerSkillBase::OnDeactivatedAsChainSkill()
{
}

void UPlayerSkillBase::StartCooldown()
{
}

void UPlayerSkillBase::FinishCooldown()
{
}

void UPlayerSkillBase::CancelCooldown()
{
}

void UPlayerSkillBase::UpdateCooldown()
{
}

float UPlayerSkillBase::GetSkillDuration() const
{
	return SkillDuration;
}
