// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "UniqueMovementSkill.h"
#include "EODCharacterBase.h"

#include "TimerManager.h"
#include "Engine/World.h"

UUniqueMovementSkill::UUniqueMovementSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSkillCanBeCharged = false;
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
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator && Instigator->Controller && Instigator->Controller->IsLocalPlayerController())
	{
		Instigator->SetIsRunning(true);
		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(MovementEndTimerHandle, this, &UUniqueMovementSkill::FinishSkill, SpecialMovementDuration, false);
	}
}

void UUniqueMovementSkill::CancelSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator && Instigator->Controller)
	{
		Instigator->SetIsRunning(false);
	}
}

void UUniqueMovementSkill::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator && Instigator->Controller)
	{
		Instigator->SetIsRunning(false);
	}
}
