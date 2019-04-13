// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSpellCastingSkill.h"
#include "EODCharacterBase.h"
#include "EODGameInstance.h"

UDynamicSpellCastingSkill::UDynamicSpellCastingSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDynamicSpellCastingSkill::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	/*
	SkillInstigator = Instigator;
	SkillOwner = Owner;
	if (Instigator)
	{
		InstigatorSkillComponent = Instigator->GetGameplaySkillsComponent();
	}
	*/

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

void UDynamicSpellCastingSkill::CancelSkill()
{
	//~ @todo stop animations
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}
}

void UDynamicSpellCastingSkill::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
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
