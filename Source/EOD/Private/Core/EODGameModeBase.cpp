// Fill out your copyright notice in the Description page of Project Settings.

#include "EODGameModeBase.h"
#include "EODSaveGame.h"
#include "GameSingleton.h"
// #include "StatusEffectsManager.h"

#include "EODPlayerController.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AEODGameModeBase::AEODGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//~ @todo change respawn delay to something more sensible
	RespawnDelay = 2.f;
}

void AEODGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();
	if (World)
	{
		/*
		if (StatusEffectsManagerClass.Get())
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			// We don't want status effects manager or combat manager to be saved into map
			SpawnInfo.ObjectFlags |= RF_Transient;
			StatusEffectsManager = World->SpawnActor<AStatusEffectsManager>(StatusEffectsManagerClass, SpawnInfo);
		}
		*/
	}
}

void AEODGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEODGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UClass* AEODGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(InController);
	if (PC)
	{
		if (PC->GetGender() == ECharacterGender::Female)
		{
			return FemalePawnClass;
		}
		else
		{
			return MalePawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AEODGameModeBase::SetOffCharacterRespawn(const FVector& RespawnLocation, const TSubclassOf<AEODCharacterBase>& CharacterClass)
{
	FRespawnInfo SpawnInfo(RespawnLocation, CharacterClass);

	UWorld* World = GetWorld();
	if (World)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &AEODGameModeBase::RespawnCharacter, SpawnInfo);
		FTimerHandle TempHandle;
		World->GetTimerManager().SetTimer(TempHandle, Delegate, RespawnDelay, false);
	}
}

void AEODGameModeBase::RespawnCharacter(FRespawnInfo RespawnInfo)
{
	UClass* RespawnClass = RespawnInfo.RespawnClass.Get();
	UWorld* World = GetWorld();
	if (RespawnClass && World)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.ObjectFlags |= RF_Transient;
		World->SpawnActor<AEODCharacterBase>(RespawnClass, RespawnInfo.RespawnLocation, FRotator::ZeroRotator, SpawnInfo);
	}
}
