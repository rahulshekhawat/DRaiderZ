// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatZoneModeBase.h"
#include "CombatManager.h"
#include "StatusEffectsManager.h"

#include "Engine/World.h"

ACombatZoneModeBase::ACombatZoneModeBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void ACombatZoneModeBase::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	// We don't want status effects manager or combat manager to be saved into map
	SpawnInfo.ObjectFlags |= RF_Transient;

    StatusEffectsManager = World->SpawnActor<AStatusEffectsManager>(AStatusEffectsManager::StaticClass(), SpawnInfo);
    CombatManager = World->SpawnActor<ACombatManager>(ACombatManager::StaticClass(), SpawnInfo);

}
