// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ActiveSkillBase.h"
#include "EODCharacterBase.h"
#include "EODGameInstance.h"

UActiveSkillBase::UActiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupersedingSkillGroup = NAME_None;
	AnimationStartSectionName = FName("Default");
}

void UActiveSkillBase::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	Super::InitSkill(Instigator, Owner);

	if (Instigator)
	{
		if (Instigator->Gender == ECharacterGender::Female)
		{
			LoadFemaleAnimations();
		}
		else
		{
			LoadMaleAnimations();
		}
	}
}

void UActiveSkillBase::Reinitialize()
{
}

void UActiveSkillBase::CancelSkill()
{
}

bool UActiveSkillBase::CanCancelSkill() const
{
	return false;
}

void UActiveSkillBase::LoadFemaleAnimations()
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

		if (AnimationsToLoad.Num() > 0)
		{
			FStreamableDelegate Delegate;
			Delegate.BindUObject(this, &UActiveSkillBase::OnFemaleAnimationsLoaded);
			GameInstance->StreamableManager.RequestAsyncLoad(AnimationsToLoad, Delegate);
		}
	}
}

void UActiveSkillBase::LoadMaleAnimations()
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

		if (AnimationsToLoad.Num() > 0)
		{
			FStreamableDelegate Delegate;
			Delegate.BindUObject(this, &UActiveSkillBase::OnMaleAnimationsLoaded);
			GameInstance->StreamableManager.RequestAsyncLoad(AnimationsToLoad, Delegate);
		}
	}
}

void UActiveSkillBase::OnFemaleAnimationsLoaded()
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
}

void UActiveSkillBase::OnMaleAnimationsLoaded()
{
	TArray<EWeaponType> Keys;
	WeaponToMaleAnimationMontageMap .GetKeys(Keys);
	for (EWeaponType Key : Keys)
	{
		TSoftObjectPtr<UAnimMontage> SoftAnimation = WeaponToMaleAnimationMontageMap[Key];
		if (SoftAnimation.IsValid())
		{
			SkillAnimations.Add(Key, SoftAnimation.Get());
		}
	}
}

bool UActiveSkillBase::IsWeaponTypeSupported(EWeaponType WeaponType) const
{
	return (SupportedWeapons & (1 << (uint8)WeaponType));
}

bool UActiveSkillBase::IsSkillInCooldown() const
{
	//~ @todo
	return false;
}
