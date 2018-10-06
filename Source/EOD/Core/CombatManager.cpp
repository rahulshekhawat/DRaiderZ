// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatManager.h"

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

void ACombatManager::OnMeleeHit(const AActor* Instigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	
}
