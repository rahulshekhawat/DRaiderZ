// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "PlayerSkillBase.h"
#include "EODCharacterBase.h"

UPlayerSkillBase::UPlayerSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentUpgrade = 0;
	MaxUpgrades = 1;
}

bool UPlayerSkillBase::CanPlayerActivateThisSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	EWeaponType EquippedWeaponType = Instigator ? Instigator->GetEquippedWeaponType() : EWeaponType::None;

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType) && !Instigator->IsWeaponSheathed();
	bool bInCooldown = IsSkillInCooldown();

	return bHasValidWeapon && !bInCooldown;
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
