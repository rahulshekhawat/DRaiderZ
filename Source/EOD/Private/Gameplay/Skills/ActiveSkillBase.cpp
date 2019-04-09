// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ActiveSkillBase.h"
#include "EODCharacterBase.h"
#include "EODGameInstance.h"
#include "PlayerSkillsComponent.h"

#include "TimerManager.h"

UActiveSkillBase::UActiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupersedingSkillGroup = NAME_None;
	AnimationStartSectionName = FName("Default");
	CooldownRemaining = 0.f;
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

void UActiveSkillBase::StartCooldown()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	check(Instigator);
	UWorld* World = Instigator->GetWorld();
	check(World);

	const FActiveSkillLevelUpInfo CurrentLevelUpInfo = GetCurrentSkillLevelupInfo();
	World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UActiveSkillBase::UpdateCooldown, 1.f, true, 0.f);
	CooldownRemaining = CurrentLevelUpInfo.Cooldown;

}

void UActiveSkillBase::FinishCooldown()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	check(Instigator);
	UWorld* World = Instigator->GetWorld();
	check(World);

	World->GetTimerManager().ClearTimer(CooldownTimerHandle);

	UGameplaySkillsComponent* SkillComp = InstigatorSkillComponent.Get();
	check(SkillComp);
	CooldownRemaining = 0.f;
	SkillComp->UpdateSkillCooldown(SkillGroup, CooldownRemaining);
}

void UActiveSkillBase::CancelCooldown()
{
	FinishCooldown();
}

void UActiveSkillBase::UpdateCooldown()
{
	if (CooldownRemaining <= 0.f)
	{
		FinishCooldown();
	}

	UGameplaySkillsComponent* SkillComp = InstigatorSkillComponent.Get();
	check(SkillComp);
	SkillComp->UpdateSkillCooldown(SkillGroup, CooldownRemaining);

	CooldownRemaining--;
}
