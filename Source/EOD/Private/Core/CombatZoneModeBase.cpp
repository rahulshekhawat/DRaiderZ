// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatZoneModeBase.h"
#include "CombatManager.h"

#include "Engine/World.h"

ACombatZoneModeBase::ACombatZoneModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CombatManagerClass = ACombatManager::StaticClass();
}

void ACombatZoneModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();
	if (World)
	{
		if (CombatManagerClass.Get())
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			// We don't want status effects manager or combat manager to be saved into map
			SpawnInfo.ObjectFlags |= RF_Transient;
			CombatManager = World->SpawnActor<ACombatManager>(CombatManagerClass, SpawnInfo);
		}
	}
}

void ACombatZoneModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatZoneModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACombatManager* ACombatZoneModeBase::BP_GetCombatManager() const
{
	return GetCombatManager();
}
