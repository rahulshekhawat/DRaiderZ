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

void UUniqueMovementSkill::TriggerSkill()
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

		//~ consume stamina and mana
		CommitSkill();
	}

	SkillDuration = FailSafeDuration;
	UWorld* World = Instigator->GetWorld();
	check(World);
	World->GetTimerManager().SetTimer(SkillTimerHandle, this, &UUniqueMovementSkill::FinishSkill, SkillDuration, false);

	Instigator->AddGameplayTagModifier(FGameplayTagMod(ActivationOwnedTags, this));
	QueueGameplayEffectEvents();
}

void UUniqueMovementSkill::ApplyRotation()
{
	//~ empty def
}
