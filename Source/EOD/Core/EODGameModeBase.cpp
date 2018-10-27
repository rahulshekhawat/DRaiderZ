// Fill out your copyright notice in the Description page of Project Settings.

#include "EODGameModeBase.h"
#include "Core/EODSaveGame.h"
#include "Core/GameSingleton.h"
#include "Core/StatusEffectsManager.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AEODGameModeBase::AEODGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AEODGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UGameSingleton* GameSingleton = nullptr;
	if (GEngine)
	{
		GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	}

	if (!GameSingleton)
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->DefaultSaveSlotName, GameSingleton->UserIndex));
	if (!EODSaveGame)
	{
		EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::CreateSaveGameObject(UEODSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(EODSaveGame, GameSingleton->DefaultSaveSlotName, GameSingleton->UserIndex);
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	// We don't want status effects manager or combat manager to be saved into map
	SpawnInfo.ObjectFlags |= RF_Transient;

    StatusEffectsManager = World->SpawnActor<AStatusEffectsManager>(AStatusEffectsManager::StaticClass(), SpawnInfo);	

}

AStatusEffectsManager* AEODGameModeBase::BP_GetStatusEffectsManager() const
{
	return GetStatusEffectsManager();
}
