// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "UniqueMovementSkill.h"
#include "EODCharacterBase.h"

UUniqueMovementSkill::UUniqueMovementSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UUniqueMovementSkill::CanTriggerSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	EWeaponType EquippedWeaponType = Instigator ? Instigator->GetEquippedWeaponType() : EWeaponType::None;

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType) && !Instigator->IsWeaponSheathed();
	bool bInCooldown = IsSkillInCooldown();

	bool bInstigatorCanUseSkill = Instigator ? Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking() : false;

	return bHasValidWeapon && !bInCooldown && bInstigatorCanUseSkill;
}

void UUniqueMovementSkill::TriggerSkill()
{
}
