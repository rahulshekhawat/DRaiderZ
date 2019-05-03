// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameplaySkillsComponent.h"
#include "EODCharacterBase.h"
#include "PlayerCharacter.h"
#include "PlayerSaveGame.h"
#include "EODGameInstance.h"
#include "GameplaySkillBase.h"
#include "EODCharacterMovementComponent.h"
#include "GameplayEffectBase.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


/** EOD Character stats */
DECLARE_CYCLE_STAT(TEXT("EOD GameplaySkillsTick"), STAT_EODGameplaySkillsTick, STATGROUP_EOD);

UGameplaySkillsComponent::UGameplaySkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	ChainSkillResetDelay = 2.f;
}

void UGameplaySkillsComponent::PostLoad()
{
	Super::PostLoad();
}

void UGameplaySkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Casting GetOwner() in BeginPlay because:
	// * GetOwner() in constructor is CDO of created character (e.g., Default_BP_FemaleCharacter, etc.)
	// * GetOwner() in PostLoad() was correct for AI characters spawned along with map, but was incorrect (NULL)  for player character
	// * GetOwner() has been found to be setup correctly in BeginPlay
	EODCharacterOwner = Cast<AEODCharacterBase>(GetOwner());

	//~ @note The result of IsPlayerControlled() is not correct during BeginPlay because PlayerState has not finished replicating.

}

void UGameplaySkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	SCOPE_CYCLE_COUNTER(STAT_EODGameplaySkillsTick);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bSkillCharging)
	{
		SkillChargeDuration += DeltaTime;
	}

	for (UGameplaySkillBase* Skill : ActiveSkills)
	{
		check(Skill);
		Skill->UpdateSkill(DeltaTime);
	}

	for (UGameplayEffectBase* Effect : ActiveGameplayEffects)
	{
		check(Effect);
		Effect->UpdateEffect(DeltaTime);
	}

	// bool bIsLocallyControlled = EODCharacterOwner && EODCharacterOwner->Controller && EODCharacterOwner->Controller->IsLocalController();
	// bool bIsLocallyControlledByPlayer = EODCharacterOwner && EODCharacterOwner->Controller && EODCharacterOwner->Controller->IsLocalPlayerController();
	// if (bIsLocallyControlled)
	// {
	// }
}

void UGameplaySkillsComponent::TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	if (!Skill)
	{
		check(SkillIndexToSkillMap.Contains(SkillIndex));
		Skill = SkillIndexToSkillMap[SkillIndex];
	}

	if (!Skill || !EODCharacterOwner)
	{
		return;
	}

	//~ @todo
}

void UGameplaySkillsComponent::ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill, float ReleaseDelay)
{
}

void UGameplaySkillsComponent::CancelSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	check(SkillIndexToSkillMap.Contains(SkillIndex));
	if (Skill == nullptr)
	{
		Skill = SkillIndexToSkillMap[SkillIndex];
	}

	if (ActiveSkills.Contains(Skill))
	{
		Skill->CancelSkill();
	}
}

void UGameplaySkillsComponent::CancelAllActiveSkills()
{
	// It's important to loop in reverse because Skill->CancelSkill() modifies the ActiveSkills array.
	int32 SkillsNum = ActiveSkills.Num();
	for (int i = SkillsNum - 1; i >= 0; i--)
	{
		UGameplaySkillBase* Skill = ActiveSkills[i];
		check(Skill);
		Skill->CancelSkill();
	}
}

bool UGameplaySkillsComponent::CanUseAnySkill() const
{
	return IsValid(EODCharacterOwner) && EODCharacterOwner->CanUseAnySkill();
}

bool UGameplaySkillsComponent::CanUseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	return false;
}

void UGameplaySkillsComponent::ActivateChainSkill(UGameplaySkillBase* CurrentSkill)
{
}

uint8 UGameplaySkillsComponent::GetSkillIndexForSkillGroup(FName SkillGroup) const
{
	return SkillGroupToSkillIndexMap.Contains(SkillGroup) ? SkillGroupToSkillIndexMap[SkillGroup] : 0;
}

UGameplaySkillBase* UGameplaySkillsComponent::GetSkillForSkillGroup(FName SkillGroup) const
{
	uint8 SkillIndex = SkillGroupToSkillIndexMap.Contains(SkillGroup) ? SkillGroupToSkillIndexMap[SkillGroup] : 0;
	UGameplaySkillBase* Skill = SkillIndexToSkillMap.Contains(SkillIndex) ? SkillIndexToSkillMap[SkillIndex] : nullptr;
	return Skill;
}

void UGameplaySkillsComponent::OnSkillCancelled(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	if (Skill)
	{
		ActiveSkills.Remove(Skill);
	}
}

void UGameplaySkillsComponent::OnSkillFinished(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	if (Skill)
	{
		ActiveSkills.Remove(Skill);
	}
}

void UGameplaySkillsComponent::ResetChainSkill()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(ChainSkillTimerHandle);
	}

	ActivePrecedingChainSkillGroup = NAME_None;
	SupersedingChainSkillGroup = TPair<uint8, uint8>(0, 0);
}

void UGameplaySkillsComponent::InitializeSkills(AEODCharacterBase* CompOwner)
{
}

void UGameplaySkillsComponent::UpdateSkillCooldown(FName SkillGroup, float RemainingCooldown)
{
}

void UGameplaySkillsComponent::UpdateSkillCooldown(uint8 SkillIndex, float RemainingCooldown)
{
}

void UGameplaySkillsComponent::AddGameplayEffect(UGameplayEffectBase* GameplayEffect)
{
	if (GameplayEffect)
	{
		ActiveGameplayEffects.Add(GameplayEffect);
		if (!GameplayEffect->IsActive())
		{
			GameplayEffect->ActivateEffect();
		}
	}
}

void UGameplaySkillsComponent::RemoveGameplayEffect(UGameplayEffectBase* GameplayEffect)
{
	if (GameplayEffect)
	{
		ActiveGameplayEffects.Remove(GameplayEffect);
		if (GameplayEffect->IsActive())
		{
			GameplayEffect->DeactivateEffect();
		}
	}
}

void UGameplaySkillsComponent::Server_TriggerSkill_Implementation(uint8 SkillIndex)
{
}

bool UGameplaySkillsComponent::Server_TriggerSkill_Validate(uint8 SkillIndex)
{
	return true;
}

void UGameplaySkillsComponent::Server_ReleaseSkill_Implementation(uint8 SkillIndex, float ChargeDuration)
{
}

bool UGameplaySkillsComponent::Server_ReleaseSkill_Validate(uint8 SkillIndex, float ChargeDuration)
{
	return true;
}
