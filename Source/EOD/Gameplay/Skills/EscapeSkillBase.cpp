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

bool UEscapeSkillBase::CanTriggerSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	EWeaponType EquippedWeaponType = Instigator ? Instigator->GetEquippedWeaponType() : EWeaponType::None;

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType);
	bool bInCooldown = IsSkillInCooldown();

	//~ @note Escape skills can be used even if the instigator has been hit
	bool bInstigatorCanUseSkill = Instigator ? Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking() || Instigator->HasBeenHit() : false;

	return bHasValidWeapon && !bInCooldown && bInstigatorCanUseSkill;
}

void UEscapeSkillBase::TriggerSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();

	bool bHasValidObjReferences = Instigator && SkillsComp; // We do not need a valid controller to trigger this skill on a character

	if (!bHasValidObjReferences)
	{
		return;
	}

	AController* Controller = SkillOwner.Get();
	bool bIsLocalPlayerController = Controller && Controller->IsLocalPlayerController();

	if (bIsLocalPlayerController)
	{
	}

	EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
	UAnimMontage* Montage = SkillAnimations.Contains(CurrentWeapon) ? SkillAnimations[CurrentWeapon] : nullptr;
	if (Montage)
	{
		Instigator->PlayAnimMontage(Montage, 1.f, AnimationStartSectionName);
	}
}

bool UEscapeSkillBase::CanReleaseSkill() const
{
	return false;
}

void UEscapeSkillBase::ReleaseSkill(float ChargeDuration)
{
}

bool UEscapeSkillBase::CanCancelSkill() const
{
	return false;
}

void UEscapeSkillBase::CancelSkill()
{
}
