// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatZoneModeBase.h"
#include "EOD/Core/CombatManager.h"

#include "Engine/World.h"

ACombatZoneModeBase::ACombatZoneModeBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	CombatManagerClass = ACombatManager::StaticClass();
}

void ACombatZoneModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();

	// @todo Spawn combat and status effect manager only on server

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	// We don't want status effects manager or combat manager to be saved into map
	SpawnInfo.ObjectFlags |= RF_Transient;

    CombatManager = World->SpawnActor<ACombatManager>(CombatManagerClass, SpawnInfo);

}

ACombatManager * ACombatZoneModeBase::BP_GetCombatManager() const
{
	return GetCombatManager();
}
