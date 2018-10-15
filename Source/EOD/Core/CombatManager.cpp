// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatManager.h"
#include "Player/EODCharacterBase.h"
#include "Player/PlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"

ACombatManager::ACombatManager(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void ACombatManager::BeginPlay()
{
	Super::BeginPlay();

}

void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatManager::OnMeleeHit(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(HitInstigator);

	if (PlayerCharacter)
	{
		ProcessPlayerMeleeAttack(PlayerCharacter, bHit, HitResults);
	}
	else
	{
		AEODCharacterBase* NPCCharacter = Cast<AEODCharacterBase>(HitInstigator);
		if (NPCCharacter)
		{
			ProcessNPCMeleeAttack(NPCCharacter, bHit, HitResults);
		}
		else
		{
			ProcessActorMeleeAttack(HitInstigator, bHit, HitResults);
		}
	}
}

void ACombatManager::ProcessActorMeleeAttack(AActor * HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	// @todo definition
}

void ACombatManager::ProcessNPCMeleeAttack(AEODCharacterBase * HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	// @todo definition
}

void ACombatManager::ProcessPlayerMeleeAttack(APlayerCharacter * HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	FPlayerSkillTableRow* PlayerSkill = HitInstigator->GetSkill(HitInstigator->GetCurrentActiveSkillID());
	check(PlayerSkill);

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.Actor.Get();
		AEODCharacterBase* HitNPC = Cast<AEODCharacterBase>(HitActor);

		if (HitNPC)
		{
			PlayerToNPCMeleeAttack(HitInstigator, HitNPC, PlayerSkill, HitResult);
		}
		else
		{
			PlayerToActorMeleeAttack(HitInstigator, HitActor, PlayerSkill, HitResult);
		}
	}
}

void ACombatManager::PlayerToNPCMeleeAttack(APlayerCharacter* HitInstigator,
											AEODCharacterBase* HitCharacter,
											const FPlayerSkillTableRow* HitSkill,
											const FHitResult& HitResult)
{
	check(HitInstigator);
	check(HitCharacter);
	check(HitSkill);

	if (!HitSkill->bUndodgable && HitCharacter->IsDodgingDamage())
	{
		DisplayDamageText(FString("Dodge"));
	}

	if (!HitSkill->bUnblockable && HitCharacter->IsBlockingDamage())
	{



	}
}

void ACombatManager::PlayerToActorMeleeAttack(APlayerCharacter* HitInstigator,
											  AActor* HitActor,
											  const FPlayerSkillTableRow* HitSkill,
											  const FHitResult& HitResult)
{
	// @todo definition
}
