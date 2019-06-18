// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ActiveSkillBase.h"
#include "EODCharacterBase.h"
#include "EODGameInstance.h"
#include "PlayerSkillsComponent.h"
#include "EODPlayerController.h"
#include "PlayerStatsComponent.h"
#include "EODCharacterMovementComponent.h"
#include "GameplayEffectBase.h"
#include "EODGlobalNames.h"

#include "TimerManager.h"

UActiveSkillBase::UActiveSkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupersedingSkillGroup			= NAME_None;
	AnimationStartSectionName		= FName("Default");
	CooldownRemaining				= 0.f;
	FailSafeDuration				= 0.5f;
	CamShakeType					= ECameraShakeType::Weak;
}

void UActiveSkillBase::InitSkill(AEODCharacterBase* Instigator, AController* Owner)
{
	Super::InitSkill(Instigator, Owner);

	check(Instigator);
	if (Instigator->Gender == ECharacterGender::Female)
	{
		LoadFemaleAnimations();
	}
	else
	{
		LoadMaleAnimations();
	}
}

bool UActiveSkillBase::CanCommitSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	AEODPlayerController* PC = Instigator ? Cast<AEODPlayerController>(Instigator->Controller) : nullptr;

	UPlayerStatsComponent* StatsComponent = PC ? PC->GetStatsComponent() : nullptr;
	if (StatsComponent)
	{
		const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();
		if (StatsComponent->Stamina.GetCurrentValue() >= LevelUpInfo.StaminaCost &&
			StatsComponent->Mana.GetCurrentValue() >= LevelUpInfo.ManaCost)
		{
			return true;
		}
	}

	return false;
}

void UActiveSkillBase::CommitSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	AEODPlayerController* PC = Instigator ? Cast<AEODPlayerController>(Instigator->Controller) : nullptr;

	UPlayerStatsComponent* StatsComponent = PC ? PC->GetStatsComponent() : nullptr;
	if (StatsComponent)
	{
		const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();

		int32 StaminaChange = -1 * LevelUpInfo.StaminaCost;
		int32 ManaChange = -1 * LevelUpInfo.ManaCost;
		StatsComponent->Stamina.ModifyCurrentValue(StaminaChange);
		StatsComponent->Mana.ModifyCurrentValue(ManaChange);
	}
}

void UActiveSkillBase::ApplyRotation()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
	if (MoveComp)
	{
		float DesiredRotationYaw = Instigator->GetControllerRotationYaw();
		MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
	}
}

bool UActiveSkillBase::CanTriggerSkill() const
{
	// Along with UPlayerSkillBase::CanTriggerSkill() implementation, also check if player has enough stats to commit this skill.
	return CanCommitSkill() && Super::CanTriggerSkill();
}

void UActiveSkillBase::TriggerSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (!Instigator)
	{
		return;
	}

	bool bIsLocalPlayerController = Instigator->Controller && Instigator->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		ApplyRotation();

		Instigator->SetCharacterStateAllowsMovement(false);
		Instigator->SetCharacterStateAllowsRotation(false);

		StartCooldown();
	}

	bool bHasController = Instigator->Controller != nullptr;
	if (bHasController)
	{
		FCharacterStateInfo StateInfo(ECharacterState::UsingActiveSkill, SkillIndex);
		StateInfo.NewReplicationIndex = Instigator->CharacterStateInfo.NewReplicationIndex + 1;
		Instigator->CharacterStateInfo = StateInfo;

		//~ consume stamina and mana
		CommitSkill();
		AcquireCCImmunities();
	}

	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->bUseControllerDesiredRotation = false;
	}

	EWeaponType CurrentWeapon = Instigator->GetEquippedWeaponType();
	UAnimMontage* Montage = SkillAnimations.Contains(CurrentWeapon) ? SkillAnimations[CurrentWeapon] : nullptr;
	if (Montage)
	{
		SkillDuration = Instigator->PlayAnimMontage(Montage, 1.f, AnimationStartSectionName);
		float ActualSkillDuration;

		if (Montage->BlendOutTriggerTime >= 0.f)
		{
			ActualSkillDuration = SkillDuration;
		}
		else
		{
			ActualSkillDuration = SkillDuration - Montage->BlendOut.GetBlendTime();
		}

		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(SkillTimerHandle, this, &UActiveSkillBase::FinishSkill, ActualSkillDuration, false);
	}
	else
	{
		SkillDuration = FailSafeDuration;
		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(SkillTimerHandle, Instigator, &AEODCharacterBase::ResetState, SkillDuration, false);
	}

	Instigator->AddGameplayTagModifier(FGameplayTagMod(ActivationOwnedTags, this));

	QueueGameplayEffectEvents();
}

bool UActiveSkillBase::CanCancelSkill() const
{
	return false;
}

void UActiveSkillBase::CancelSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().ClearTimer(SkillTimerHandle);
	}

	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if (SkillsComp)
	{
		SkillsComp->OnSkillCancelled(SkillIndex, SkillGroup, this);
	}

	// Instigator->RemoveGameplayTagModifier(FGameplayTagMod(ActivationOwnedTags, this));

	DisableGameplayEffectEvents();
	LoseCCImmunities();
}

void UActiveSkillBase::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}

	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if (SkillsComp)
	{
		SkillsComp->OnSkillFinished(SkillIndex, SkillGroup, this);
	}

	// Instigator->RemoveGameplayTagModifier(FGameplayTagMod(ActivationOwnedTags, this));

	DisableGameplayEffectEvents();
	LoseCCImmunities();
}

void UActiveSkillBase::QueueGameplayEffectEvents()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();

	const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();
	UClass* GameplayEffectClass = LevelUpInfo.GameplayEffectInfo.Class.Get();
	UGameplaySkillsComponent* SkillsComponent = InstigatorSkillComponent.Get();
	if (GameplayEffectClass && SkillsComponent)
	{
		FGameplayEventInfo EventInfo;
		EventInfo.EventClass = GameplayEffectClass;
		EventInfo.EventClassType = EGameplayEventClassType::GameplayEffect;
		EventInfo.EventSubIndex = LevelUpInfo.GameplayEffectInfo.Level;
		EventInfo.Instigator = Instigator;
		EventInfo.Targets.Add(Instigator);
		EventInfo.bDetermineTargetsDynamically = false;

		FName TriggerCondition = LevelUpInfo.GameplayEffectInfo.TriggerCondition;
		if (SkillsComponent->GameplayEvents.Contains(TriggerCondition))
		{
			SkillsComponent->GameplayEvents[TriggerCondition].Add(this, EventInfo);
		}
		else
		{
			SkillsComponent->GameplayEvents.Add(TriggerCondition);
			SkillsComponent->GameplayEvents[TriggerCondition].Add(this, EventInfo);
		}
	}
}

void UActiveSkillBase::DisableGameplayEffectEvents()
{
	UGameplaySkillsComponent* SkillsComponent = InstigatorSkillComponent.Get();

	const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();
	UClass* GameplayEffectClass = LevelUpInfo.GameplayEffectInfo.Class.Get();
	if (SkillsComponent && GameplayEffectClass)
	{
		FName TriggerCondition = LevelUpInfo.GameplayEffectInfo.TriggerCondition;
		if (SkillsComponent->GameplayEvents.Contains(TriggerCondition))
		{
			SkillsComponent->GameplayEvents[TriggerCondition].Remove(this);
		}
	}
}

void UActiveSkillBase::AcquireCCImmunities()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	check(Instigator);

	const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();
	if (LevelUpInfo.CrowdControlImmunities != 0)
	{
		AEODPlayerController* PC = Cast<AEODPlayerController>(Instigator->Controller);
		UPlayerStatsComponent* StatsComp = PC ? PC->GetStatsComponent() : nullptr;
		if (StatsComp)
		{
			FCCImmunityModifier CCImmunityMod(LevelUpInfo.CrowdControlImmunities, ECCImmunityModType::Additive);
			StatsComp->CCImmunities.AddModifier(this, CCImmunityMod);
		}
	}
}

void UActiveSkillBase::LoseCCImmunities()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	check(Instigator);

	const FActiveSkillLevelUpInfo LevelUpInfo = GetCurrentSkillLevelupInfo();
	if (LevelUpInfo.CrowdControlImmunities != 0)
	{
		AEODPlayerController* PC = Cast<AEODPlayerController>(Instigator->Controller);
		UPlayerStatsComponent* StatsComp = PC ? PC->GetStatsComponent() : nullptr;
		if (StatsComp)
		{
			StatsComp->CCImmunities.RemoveModifier(this);
		}
	}
}

TSharedPtr<FAttackInfo> UActiveSkillBase::GetAttackInfoPtr(int32 CollisionIndex)
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator->Role < ROLE_Authority)
	{
		return TSharedPtr<FAttackInfo>();
	}

	AEODPlayerController* PC = Instigator ? Cast<AEODPlayerController>(Instigator->Controller) : nullptr;
	UPlayerStatsComponent* StatsComp = PC ? PC->GetStatsComponent() : nullptr;
	check(StatsComp);

	const FActiveSkillLevelUpInfo SkillInfo = GetCurrentSkillLevelupInfo();
	float CritRate = DamageType == EDamageType::Magickal ? StatsComp->MagickalCritRate.GetValue() : StatsComp->PhysicalCritRate.GetValue();
	float NormalDamage =
		DamageType == EDamageType::Magickal ?
		((SkillInfo.DamagePercent / 100.f) * StatsComp->MagickalAttack.GetValue()) :
		((SkillInfo.DamagePercent / 100.f) * StatsComp->PhysicalAttack.GetValue());
	float CritDamage =
		DamageType == EDamageType::Magickal ?
		(NormalDamage * UCombatLibrary::MagickalCritMultiplier + StatsComp->MagickalCritBonus.GetValue()) :
		(NormalDamage * UCombatLibrary::PhysicalCritMultiplier + StatsComp->PhysicalCritBonus.GetValue());

	TSharedPtr<FAttackInfo> AttackInfoPtr =
		MakeShareable(new FAttackInfo(
			SkillInfo.bUndodgable,
			SkillInfo.bUnblockable,
			CritRate,
			NormalDamage,
			CritDamage,
			DamageType,
			SkillInfo.CrowdControlEffect,
			SkillInfo.CrowdControlEffectDuration,
			CamShakeType
		));

	return AttackInfoPtr;
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

void UActiveSkillBase::StartCooldown()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	check(Instigator);
	UWorld* World = Instigator->GetWorld();
	check(World);

	const FActiveSkillLevelUpInfo CurrentLevelUpInfo = GetCurrentSkillLevelupInfo();
	World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UActiveSkillBase::UpdateCooldown, 1.f, true);
	CooldownRemaining = CurrentLevelUpInfo.Cooldown;

	UGameplaySkillsComponent* SkillComp = InstigatorSkillComponent.Get();
	check(SkillComp);
	SkillComp->UpdateSkillCooldown(SkillGroup, CooldownRemaining);
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
	CooldownRemaining--;

	if (CooldownRemaining <= 0.f)
	{
		FinishCooldown();
	}

	UGameplaySkillsComponent* SkillComp = InstigatorSkillComponent.Get();
	check(SkillComp);
	SkillComp->UpdateSkillCooldown(SkillGroup, CooldownRemaining);
}
