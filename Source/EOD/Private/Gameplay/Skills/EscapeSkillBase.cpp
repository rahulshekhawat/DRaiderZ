// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EscapeSkillBase.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"
#include "EODCharacterMovementComponent.h"
#include "GameplayEffectBase.h"

#include "TimerManager.h"
#include "Engine/World.h"

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

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType) && !Instigator->IsWeaponSheathed();
	bool bInCooldown = IsSkillInCooldown();

	//~ @note Escape skills can be used even if the instigator has been hit
	bool bInstigatorCanUseSkill = Instigator ? Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking() || Instigator->HasBeenHit() : false;

	return bHasValidWeapon && !bInCooldown && bInstigatorCanUseSkill;
}

void UEscapeSkillBase::ReleaseSkill(float ChargeDuration)
{
}

bool UEscapeSkillBase::CanCancelSkill() const
{
	return true;
}

void UEscapeSkillBase::CancelSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().ClearTimer(SkillTimerHandle);
	}
	
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillCancelled(SkillIndex, SkillGroup, this);
	}
}

void UEscapeSkillBase::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}
	
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillFinished(SkillIndex, SkillGroup, this);
	}
}

void UEscapeSkillBase::ApplyRotation()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
	if (MoveComp)
	{
		float DesiredRotationYaw = Instigator->GetControllerRotationYaw();
		MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
		Instigator->SetCharacterRotationYaw(DesiredRotationYaw);
	}
}
