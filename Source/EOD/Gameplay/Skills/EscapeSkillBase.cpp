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



	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
		UAnimMontage* Montage = SkillAnimations.Contains(CurrentWeapon) ? SkillAnimations[CurrentWeapon] : nullptr;
		if (Montage)
		{
			Instigator->PlayAnimationMontage(Montage, AnimationStartSectionName, ECharacterState::UsingActiveSkill);
		}
	}
}

void UEscapeSkillBase::CancelSkill()
{
}

void UEscapeSkillBase::EndSkill()
{
}

bool UEscapeSkillBase::CanActivateSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	EWeaponType EquippedWeaponType = Instigator ? Instigator->GetEquippedWeaponType() : EWeaponType::None;

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType);
	bool bInCooldown = IsSkillInCooldown();

	//~ @note Escape skills can be used even if the instigator has been hit
	bool bInstigatorCanUseSkill = Instigator ? Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking() || Instigator->HasBeenHit() : false;

	return bHasValidWeapon && !bInCooldown && bInstigatorCanUseSkill;
}
