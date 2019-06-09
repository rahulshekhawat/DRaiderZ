// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSpellCastingSkill.h"
#include "EODCharacterBase.h"
#include "EODGameInstance.h"
#include "PlayerAnimInstance.h"
#include "GameplaySkillsComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"

UDynamicSpellCastingSkill::UDynamicSpellCastingSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bMovableMontagePlaying = false;
	bNeedsUpdate = true;
}

void UDynamicSpellCastingSkill::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	Super::InitSkill(Instigator, Owner);
}

bool UDynamicSpellCastingSkill::CanTriggerSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	EWeaponType EquippedWeaponType = Instigator ? Instigator->GetEquippedWeaponType() : EWeaponType::None;

	bool bHasValidWeapon = EquippedWeaponType != EWeaponType::None && IsWeaponTypeSupported(EquippedWeaponType) && !Instigator->IsWeaponSheathed();
	bool bInCooldown = IsSkillInCooldown();

	bool bInstigatorCanUseSkill = Instigator ? Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking() : false;

	return bHasValidWeapon && !bInCooldown && bInstigatorCanUseSkill;
}

void UDynamicSpellCastingSkill::TriggerSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (!Instigator)
	{
		return;
	}

	EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
	check(SkillAnimations.Contains(CurrentWeapon) && SkillUpperSlotAnimations.Contains(CurrentWeapon));

	bool bIsLocalPlayerController = Instigator->Controller && Instigator->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		Instigator->UpdatePCTryingToMove();

		Instigator->SetCharacterStateAllowsMovement(true);
		Instigator->SetCharacterStateAllowsRotation(true);

		StartCooldown();
	}

	bool bHasController = Instigator->Controller != nullptr;
	if (bHasController)
	{
		FCharacterStateInfo StateInfo(ECharacterState::UsingActiveSkill, SkillIndex);
		StateInfo.NewReplicationIndex = Instigator->CharacterStateInfo.NewReplicationIndex + 1;
		Instigator->CharacterStateInfo = StateInfo;
	}

	UAnimMontage* MontageToPlay = Instigator->IsPCTryingToMove() ? SkillUpperSlotAnimations[CurrentWeapon] : SkillAnimations[CurrentWeapon];
	if (MontageToPlay)
	{
		SkillDuration = Instigator->PlayAnimMontage(MontageToPlay, 1.f, AnimationStartSectionName);
		float ActualSkillDuration;

		if (MontageToPlay->BlendOutTriggerTime >= 0.f)
		{
			ActualSkillDuration = SkillDuration;
		}
		else
		{
			ActualSkillDuration = SkillDuration - MontageToPlay->BlendOut.GetBlendTime();
		}

		bMovableMontagePlaying = Instigator->IsPCTryingToMove();

		UWorld* World = Instigator->GetWorld();
		check(World);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UDynamicSpellCastingSkill::FinishSkill);
		World->GetTimerManager().SetTimer(SkillTimerHandle, TimerDelegate, ActualSkillDuration, false);
	}
	else
	{
		Instigator->ResetState();
	}
}

bool UDynamicSpellCastingSkill::CanCancelSkill() const
{
	return false;
}

void UDynamicSpellCastingSkill::UpdateSkill(float DeltaTime)
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (!Instigator)
	{
		return;
	}

	if (Instigator->IsPCTryingToMove() != bMovableMontagePlaying)
	{
		EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
		check(SkillAnimations.Contains(CurrentWeapon) && SkillUpperSlotAnimations.Contains(CurrentWeapon));
		UAnimMontage* FullSlotAnimation = SkillAnimations[CurrentWeapon];
		UAnimMontage* UpperSlotAnimation = SkillUpperSlotAnimations[CurrentWeapon];

		UAnimInstance* PrimaryAnimInstance = Instigator->GetMesh() ? Instigator->GetMesh()->GetAnimInstance() : nullptr;

		if (PrimaryAnimInstance && FullSlotAnimation && UpperSlotAnimation)
		{
			if (Instigator->IsPCTryingToMove())
			{
				TransitionBetweenMontages(PrimaryAnimInstance, FullSlotAnimation, UpperSlotAnimation);
				bMovableMontagePlaying = true;
			}
			else
			{
				TransitionBetweenMontages(PrimaryAnimInstance, UpperSlotAnimation, FullSlotAnimation);
				bMovableMontagePlaying = false;
			}
		}
	}
}

void UDynamicSpellCastingSkill::CancelSkill()
{
	//~ @todo stop animations
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}
	
	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if(SkillsComp)
	{
		SkillsComp->OnSkillCancelled(SkillIndex, SkillGroup, this);
	}
}

void UDynamicSpellCastingSkill::FinishSkill()
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

void UDynamicSpellCastingSkill::LoadFemaleAnimations()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UEODGameInstance* GameInstance = Instigator ? Cast<UEODGameInstance>(Instigator->GetGameInstance()) : nullptr;
	if (GameInstance)
	{
		TArray<FSoftObjectPath> AnimationsToLoad;
		TArray<EWeaponType> Keys;
		WeaponToFemaleAnimationMontageMap.GetKeys(Keys);
		for (EWeaponType Key : Keys)
		{
			FSoftObjectPath ObjectPath = WeaponToFemaleAnimationMontageMap[Key].ToSoftObjectPath();
			if (ObjectPath.IsValid())
			{
				AnimationsToLoad.AddUnique(ObjectPath);
			}
		}

		WeaponToFemaleUpperSlotAnimationsMap.GetKeys(Keys);
		for (EWeaponType Key : Keys)
		{
			FSoftObjectPath ObjectPath = WeaponToFemaleUpperSlotAnimationsMap[Key].ToSoftObjectPath();
			if (ObjectPath.IsValid())
			{
				AnimationsToLoad.AddUnique(ObjectPath);
			}
		}

		if (AnimationsToLoad.Num() > 0)
		{
			FStreamableDelegate Delegate;
			Delegate.BindUObject(this, &UDynamicSpellCastingSkill::OnFemaleAnimationsLoaded);
			GameInstance->StreamableManager.RequestAsyncLoad(AnimationsToLoad, Delegate);
		}
	}
}

void UDynamicSpellCastingSkill::LoadMaleAnimations()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UEODGameInstance* GameInstance = Instigator ? Cast<UEODGameInstance>(Instigator->GetGameInstance()) : nullptr;
	if (GameInstance)
	{
		TArray<FSoftObjectPath> AnimationsToLoad;
		TArray<EWeaponType> Keys;
		WeaponToMaleAnimationMontageMap.GetKeys(Keys);
		for (EWeaponType Key : Keys)
		{
			FSoftObjectPath ObjectPath = WeaponToMaleAnimationMontageMap[Key].ToSoftObjectPath();
			if (ObjectPath.IsValid())
			{
				AnimationsToLoad.AddUnique(ObjectPath);
			}
		}

		WeaponToMaleUpperSlotAnimationsMap.GetKeys(Keys);
		for (EWeaponType Key : Keys)
		{
			FSoftObjectPath ObjectPath = WeaponToMaleUpperSlotAnimationsMap[Key].ToSoftObjectPath();
			if (ObjectPath.IsValid())
			{
				AnimationsToLoad.AddUnique(ObjectPath);
			}
		}

		if (AnimationsToLoad.Num() > 0)
		{
			FStreamableDelegate Delegate;
			Delegate.BindUObject(this, &UDynamicSpellCastingSkill::OnMaleAnimationsLoaded);
			GameInstance->StreamableManager.RequestAsyncLoad(AnimationsToLoad, Delegate);
		}
	}
}

void UDynamicSpellCastingSkill::OnFemaleAnimationsLoaded()
{
	TArray<EWeaponType> Keys;
	WeaponToFemaleAnimationMontageMap.GetKeys(Keys);
	for (EWeaponType Key : Keys)
	{
		TSoftObjectPtr<UAnimMontage> SoftAnimation = WeaponToFemaleAnimationMontageMap[Key];
		if (SoftAnimation.IsValid())
		{
			SkillAnimations.Add(Key, SoftAnimation.Get());
		}
	}

	WeaponToFemaleUpperSlotAnimationsMap.GetKeys(Keys);
	for (EWeaponType Key : Keys)
	{
		TSoftObjectPtr<UAnimMontage> SoftAnimation = WeaponToFemaleUpperSlotAnimationsMap[Key];
		if (SoftAnimation.IsValid())
		{
			SkillUpperSlotAnimations.Add(Key, SoftAnimation.Get());
		}
	}
}

void UDynamicSpellCastingSkill::OnMaleAnimationsLoaded()
{
	TArray<EWeaponType> Keys;
	WeaponToMaleAnimationMontageMap.GetKeys(Keys);
	for (EWeaponType Key : Keys)
	{
		TSoftObjectPtr<UAnimMontage> SoftAnimation = WeaponToMaleAnimationMontageMap[Key];
		if (SoftAnimation.IsValid())
		{
			SkillAnimations.Add(Key, SoftAnimation.Get());
		}
	}

	WeaponToMaleUpperSlotAnimationsMap.GetKeys(Keys);
	for (EWeaponType Key : Keys)
	{
		TSoftObjectPtr<UAnimMontage> SoftAnimation = WeaponToMaleUpperSlotAnimationsMap[Key];
		if (SoftAnimation.IsValid())
		{
			SkillUpperSlotAnimations.Add(Key, SoftAnimation.Get());
		}
	}
}

void UDynamicSpellCastingSkill::TransitionBetweenMontages(UAnimInstance* AnimInstance, UAnimMontage* TransitionFromMontage, UAnimMontage* TransitionToMontage)
{
	check(AnimInstance);
	if (AnimInstance->Montage_IsPlaying(TransitionFromMontage))
	{
		int32 MovingMontageSectionIndex = TransitionToMontage->GetSectionIndex(AnimationStartSectionName);
		int32 StandStillMontageSectionIndex = TransitionFromMontage->GetSectionIndex(AnimationStartSectionName);
		if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
		{
			float SectionStartTime;
			float SectionEndtime;
			TransitionFromMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
			float CurrentPosition = AnimInstance->Montage_GetPosition(TransitionFromMontage);
			float SectionOffset = CurrentPosition - SectionStartTime;
			AnimInstance->Montage_Stop(TransitionFromMontage->BlendOut.GetBlendTime(), TransitionFromMontage);

			TransitionToMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
			float MontageStartPosition = SectionStartTime + SectionOffset;

			AnimInstance->Montage_Play(TransitionToMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
		}
	}
}
