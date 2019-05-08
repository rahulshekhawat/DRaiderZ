// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EscapeSkillBase.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"
#include "EODCharacterMovementComponent.h"
#include "GameplayEffectBase.h"

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

void UEscapeSkillBase::TriggerSkill()
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
		Instigator->SetCharacterRotationYaw(DesiredRotationYaw);
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
		}

		Instigator->SetCharacterStateAllowsMovement(false);
		Instigator->SetCharacterStateAllowsRotation(false);

		//~ @todo consume stamina and mana

		StartCooldown();
	}

	EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
	UAnimMontage* Montage = SkillAnimations.Contains(CurrentWeapon) ? SkillAnimations[CurrentWeapon] : nullptr;
	if (Montage)
	{
		SkillDuration = Instigator->PlayAnimMontage(Montage, 1.f, AnimationStartSectionName);
		float ActualSkillDuration = SkillDuration - Montage->GetDefaultBlendOutTime();
		UWorld* World = Instigator->GetWorld();
		check(World);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UEscapeSkillBase::FinishSkill);
		World->GetTimerManager().SetTimer(SkillTimerHandle, TimerDelegate, ActualSkillDuration, false);
	}
	else
	{
		Instigator->ResetState();
	}

	const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();
	UClass* GameplayEffectClass = LevelUpInfo.GameplayEffectClass.Get();
	UGameplaySkillsComponent* SkillsComponent = InstigatorSkillComponent.Get();
	if (GameplayEffectClass != nullptr && SkillsComponent != nullptr)
	{
		if (LevelUpInfo.GameplayEffectTriggerCondition == ESkillEventTriggerCondition::TriggersOnSkillFinish)
		{
			FGameplayEventInfo EventInfo;
			EventInfo.EventClass = GameplayEffectClass;
			EventInfo.EventClassType = EGameplayEventClassType::GameplayEffect;
			EventInfo.Instigator = Instigator;
			EventInfo.Targets.Add(Instigator);
			EventInfo.bDetermineTargetsDynamically = false;
			SkillsComponent->EventsOnSkillFinished.Add(this, EventInfo);
		}
	}
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
