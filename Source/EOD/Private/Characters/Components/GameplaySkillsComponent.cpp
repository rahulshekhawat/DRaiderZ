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
		if (Skill && Skill->bNeedsUpdate)
		{
			Skill->UpdateSkill(DeltaTime);
		}
	}

	for (UGameplayEffectBase* Effect : ActiveGameplayEffects)
	{
		if (Effect && Effect->bNeedsUpdate)
		{
			Effect->UpdateEffect(DeltaTime);
		}
	}
}

void UGameplaySkillsComponent::TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
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

void UGameplaySkillsComponent::CancelSkillsWithTag(FGameplayTag Tag)
{
	// It's important to loop in reverse because Skill->CancelSkill() modifies the ActiveSkills array.
	int32 SkillsNum = ActiveSkills.Num();
	for (int i = SkillsNum - 1; i >= 0; i--)
	{
		UGameplaySkillBase* Skill = ActiveSkills[i];
		if (Skill->AbilityTags.HasTag(Tag))
		{
			Skill->CancelSkill();
		}
	}
}

bool UGameplaySkillsComponent::CanUseAnySkill() const
{
	return IsValid(EODCharacterOwner) && EODCharacterOwner->CanUseAnySkill();
}

bool UGameplaySkillsComponent::CanUseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	check(SkillIndexToSkillMap.Contains(SkillIndex));
	if (Skill == nullptr)
	{
		Skill = SkillIndexToSkillMap[SkillIndex];
	}

	return Skill ? Skill->CanTriggerSkill() : false;
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
	return SkillGroupToSkillMap.Contains(SkillGroup) ? SkillGroupToSkillMap[SkillGroup] : nullptr;
}

void UGameplaySkillsComponent::OnSkillCancelled(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	if (!Skill)
	{
		return;
	}

	ActiveSkills.Remove(Skill);

	BroadcastGameplayEvents(EventNames::OnSkillCancelled, Skill);
}

void UGameplaySkillsComponent::OnSkillFinished(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	if (!Skill)
	{
		return;
	}

	ActiveSkills.Remove(Skill);

	BroadcastGameplayEvents(EventNames::OnSkillFinished, Skill);
}

void UGameplaySkillsComponent::BroadcastGameplayEvents(FName EventType, UGameplaySkillBase* SourceSkill)
{
	if (!GameplayEvents.Contains(EventType))
	{
		return;
	}

	TMap<UGameplaySkillBase*, FGameplayEventInfo>& Events = GameplayEvents[EventType];

	if (!Events.Contains(SourceSkill))
	{
		return;
	}

	const FGameplayEventInfo& EventInfo = Events[SourceSkill];
	if (EventInfo.EventClassType == EGameplayEventClassType::GameplayEffect)
	{
		ActivateGameplayEffect(EventInfo.EventClass, EventInfo.Instigator, EventInfo.Targets, EventInfo.bDetermineTargetsDynamically);
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

AEODCharacterBase* UGameplaySkillsComponent::GetCharacterOwner()
{
	if (EODCharacterOwner)
	{
		return EODCharacterOwner;
	}
	else
	{
		EODCharacterOwner = Cast<AEODCharacterBase>(GetOwner());
		return EODCharacterOwner;
	}

	return nullptr;
}

void UGameplaySkillsComponent::ActivateGameplayEffect(UClass* GameplayEffectClass, AActor* Instigator, TArray<AActor*> Targets, bool bDetermineTargetDynamically)
{
	UGameplayEffectBase* GameplayEffect = NewObject<UGameplayEffectBase>(this, GameplayEffectClass, NAME_None, RF_Transient);
	check(GameplayEffect);

	TArray<AEODCharacterBase*> TargetChars;
	AEODCharacterBase* InstigatorChar = Cast<AEODCharacterBase>(Instigator);
	for (AActor* Target : Targets)
	{
		AEODCharacterBase* TargetChar = Cast<AEODCharacterBase>(Target);
		TargetChars.Add(TargetChar);
	}

	GameplayEffect->InitEffect(InstigatorChar, TargetChars);
	GameplayEffect->ActivateEffect();
	AddGameplayEffect(GameplayEffect);
}

bool UGameplaySkillsComponent::IsGameplayEffectTypeActive(TSubclassOf<UGameplayEffectBase> GameplayEffectClass, UGameplayEffectBase* GameplayEffectToIgnore)
{
	// Following line is just for future syntax reference
	// TArray<UGameplayEffectBase*> Results = ActiveGameplayEffects.FilterByPredicate([&](UGameplayEffectBase* GE) { return GE->IsA(GameplayEffectClass); });

	if (GameplayEffectToIgnore)
	{
		for (UGameplayEffectBase* GameplayEffect : ActiveGameplayEffects)
		{
			if (GameplayEffect->IsA(GameplayEffectClass))
			{
				return true;
			}
		}
	}
	else
	{
		for (UGameplayEffectBase* GameplayEffect : ActiveGameplayEffects)
		{
			if (GameplayEffect != GameplayEffectToIgnore && GameplayEffect->IsA(GameplayEffectClass))
			{
				return true;
			}
		}
	}

	return false;
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
