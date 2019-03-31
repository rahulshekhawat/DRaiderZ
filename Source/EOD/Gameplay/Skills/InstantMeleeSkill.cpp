// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InstantMeleeSkill.h"
#include "EODCharacterBase.h"
#include "Components/EODCharacterMovementComponent.h"

UInstantMeleeSkill::UInstantMeleeSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UInstantMeleeSkill::CanTriggerSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	EWeaponType EquippedWeaponType = Instigator ? Instigator->GetEquippedWeaponType() : EWeaponType::None;

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType) && !Instigator->IsWeaponSheathed();
	bool bInCooldown = IsSkillInCooldown();

	//~ @note Escape skills can be used even if the instigator has been hit
	bool bInstigatorCanUseSkill = Instigator ? Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking() || Instigator->HasBeenHit() : false;

	return bHasValidWeapon && !bInCooldown && bInstigatorCanUseSkill;
}

void UInstantMeleeSkill::TriggerSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (!Instigator)
	{
		return;
	}

	bool bIsLocalPlayerController = Instigator->Controller && Instigator->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		float DesiredRotationYaw = Instigator->GetControllerRotationYaw();
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
		}

		Instigator->SetCharacterStateAllowsMovement(false);
		Instigator->SetCharacterStateAllowsRotation(false);
	}

	EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
	UAnimMontage* Montage = SkillAnimations.Contains(CurrentWeapon) ? SkillAnimations[CurrentWeapon] : nullptr;
	if (Montage)
	{
		float SkillDuration = Instigator->PlayAnimMontage(Montage, 1.f, AnimationStartSectionName);
		SkillDuration = SkillDuration - Montage->GetDefaultBlendOutTime();
		UWorld* World = Instigator->GetWorld();
		check(World);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UInstantMeleeSkill::FinishSkill);
		World->GetTimerManager().SetTimer(SkillTimerHandle, TimerDelegate, SkillDuration, false);
	}
	else
	{
		Instigator->ResetState();
	}
}

bool UInstantMeleeSkill::CanCancelSkill() const
{
	return true;
}

void UInstantMeleeSkill::CancelSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().ClearTimer(SkillTimerHandle);
	}
}

void UInstantMeleeSkill::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}
}
