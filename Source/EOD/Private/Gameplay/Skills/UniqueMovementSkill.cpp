// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "UniqueMovementSkill.h"
#include "EODCharacterBase.h"
#include "GameplayEffectBase.h"
#include "GameplaySkillsComponent.h"

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

	//~ @todo
	/*
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();

	if (!Instigator || !SkillsComp)
	{
		return;
	}

	bool bIsLocalPlayerController = Instigator->Controller && Instigator->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		FActiveSkillLevelUpInfo SkillInfo = GetCurrentSkillLevelupInfo();
		UClass* GameplayEffectClass = SkillInfo.GameplayEffectSoftClass.Get();
		if (GameplayEffectClass)
		{
			UGameplayEffectBase* GameplayEffect = NewObject<UGameplayEffectBase>(SkillsComp, GameplayEffectClass, NAME_None, RF_Transient);
			check(GameplayEffect);
			GameplayEffect->InitEffect(Instigator, Instigator);
			SkillsComp->AddGameplayEffect(GameplayEffect);
		}
	}
	*/
}

void UUniqueMovementSkill::CancelSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator && Instigator->Controller)
	{
		Instigator->SetIsRunning(false);
	}

	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillCancelled(SkillIndex, SkillGroup, this);
	}
}

void UUniqueMovementSkill::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator && Instigator->Controller)
	{
		Instigator->SetIsRunning(false);
	}
	
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillFinished(SkillIndex, SkillGroup, this);
	}
}
