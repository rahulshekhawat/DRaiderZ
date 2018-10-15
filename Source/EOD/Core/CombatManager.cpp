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

void ACombatManager::OnMeleeHit(AActor* Instigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Instigator);

	if (PlayerCharacter)
	{
		ProcessPlayerMeleeAttack(PlayerCharacter, bHit, HitResults);
	}
	else
	{
		AEODCharacterBase* NPCCharacter = Cast<AEODCharacterBase>(Instigator);
		if (NPCCharacter)
		{
			ProcessNPCMeleeAttack(NPCCharacter, bHit, HitResults);
		}
		else
		{
			ProcessActorMeleeAttack(Instigator, bHit, HitResults);
		}
	}
}

void ACombatManager::ProcessActorMeleeAttack(AActor * Instigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
}

void ACombatManager::ProcessNPCMeleeAttack(AEODCharacterBase * Instigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
}

void ACombatManager::ProcessPlayerMeleeAttack(APlayerCharacter * Instigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	FPlayerSkillTableRow* PlayerSkill = Instigator->GetSkill(Instigator->GetCurrentActiveSkillID());
	check(PlayerSkill);

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.Actor.Get();
		AEODCharacterBase* HitNPC = Cast<AEODCharacterBase>(HitActor);

		if (HitNPC)
		{
			PlayerToNPCMeleeAttack(Instigator, HitNPC, PlayerSkill, HitResult);
		}
		else
		{
			PlayerToActorMeleeAttack(Instigator, HitActor, PlayerSkill, HitResult);
		}
	}
}

void ACombatManager::PlayerToNPCMeleeAttack(APlayerCharacter* Instigator,
											AEODCharacterBase* HitCharacter,
											const FPlayerSkillTableRow* HitSkill,
											const FHitResult& HitResult)
{
	check(Instigator);
	check(HitCharacter);
	check(HitSkill);

	if (!HitSkill->bUndodgable && HitCharacter->IsDodgingDamage())
	{
		DisplayDamageText(FString("Dodge"));
	}


}

void ACombatManager::PlayerToActorMeleeAttack(APlayerCharacter* Instigator,
											  AActor* HitActor,
											  const FPlayerSkillTableRow* HitSkill,
											  const FHitResult& HitResult)
{
}
