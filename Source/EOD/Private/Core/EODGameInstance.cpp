// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODGameInstance.h"
#include "EODPreprocessors.h"
#include "EODPlayerController.h"
#include "PlayerSkillTreeManager.h"
#include "MetaSaveGame.h"
#include "PlayerSaveGame.h"
#include "EODGlobalNames.h"

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Modules/ModuleManager.h"
#include "Kismet/GameplayStatics.h"


const int32 UEODGameInstance::PlayerIndex(0);
const FString UEODGameInstance::MetaSaveSlotName(TEXT("MetaSaveSlot"));

UEODGameInstance::UEODGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameTitle = FText::FromString("Dark RaiderZ");
	StartupMapName = FName("Level0_Haddon");
}

void UEODGameInstance::Init()
{
	Super::Init();

	// Load the meta save game object
	if (UGameplayStatics::DoesSaveGameExist(UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex))
	{
		MetaSaveGame = Cast<UMetaSaveGame>(UGameplayStatics::LoadGameFromSlot(UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex));
	}
	// If the meta save game object doesn't exist yet (running game for first time), create and save one 
	else
	{
		MetaSaveGame = Cast<UMetaSaveGame>(UGameplayStatics::CreateSaveGameObject(UMetaSaveGame::StaticClass()));
		if (IsValid(MetaSaveGame))
		{
			UGameplayStatics::SaveGameToSlot(MetaSaveGame, UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex);
		}
	}

	if (IsValid(MetaSaveGame))
	{
		if (MetaSaveGame->LastUsedSlotName != FString("") && UGameplayStatics::DoesSaveGameExist(MetaSaveGame->LastUsedSlotName, UEODGameInstance::PlayerIndex))
		{
			CurrentProfileSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(MetaSaveGame->LastUsedSlotName, UEODGameInstance::PlayerIndex));
			CurrentProfileName = MetaSaveGame->LastUsedSlotName;
		}
#if WITH_EDITOR
		else
		{
			CreateNewProfile(FString("Editor_SaveGame"));
			LoadProfileAsCurrent(FString("Editor_SaveGame"));
		}
#endif
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UEODGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UEODGameInstance::OnPostLoadMap);

}

void UEODGameInstance::StartNewCampaign()
{
	UGameplayStatics::OpenLevel(this, StartupMapName);
}

void UEODGameInstance::ContinuePreviousCampaign()
{
}

bool UEODGameInstance::CanContinuePreviousCampaign() const
{
	return false;
}

void UEODGameInstance::CreateNewProfile(const FString& ProfileName)
{
	if (IsValid(MetaSaveGame))
	{
		UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
		if (IsValid(PlayerSaveGame))
		{
			bool bResult = UGameplayStatics::SaveGameToSlot(PlayerSaveGame, ProfileName, UEODGameInstance::PlayerIndex);
			if (bResult)
			{
				FMetaSaveGameData TempMSGData;
				TempMSGData.SaveSlotName = ProfileName;
				TempMSGData.CharacterName = ProfileName;
				TempMSGData.PlayerIndex = UEODGameInstance::PlayerIndex;
				TempMSGData.LastSaveTime = FDateTime::Now();

				MetaSaveGame->SaveSlotMetaDataList.Add(TempMSGData);
				UGameplayStatics::SaveGameToSlot(MetaSaveGame, UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex);
			}
		}
	}

}

UPlayerSaveGame* UEODGameInstance::LoadProfileAsCurrent(const FString& ProfileName)
{
	if (ProfileName == CurrentProfileName)
	{
		return CurrentProfileSaveGame;
	}
	else
	{
		if (IsValid(MetaSaveGame))
		{
			UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(ProfileName, UEODGameInstance::PlayerIndex));
			if (IsValid(PlayerSaveGame))
			{
				CurrentProfileSaveGame = PlayerSaveGame;
				CurrentProfileName = ProfileName;
				MetaSaveGame->LastUsedSlotName = ProfileName;
				UGameplayStatics::SaveGameToSlot(MetaSaveGame, UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex);
				return CurrentProfileSaveGame;
			}
		}
	}

	return nullptr;
}

void UEODGameInstance::OnPreLoadMap(const FString& MapName)
{
	//~ @todo
	/*
	if (SkillTreeManager)
	{
		SkillTreeManager->Destroy();
	}
	*/

	// if (GetMoviePlayer()->IsStartupMoviePlaying() || IsRunningDedicatedServer())
	if (GetMoviePlayer()->IsStartupMoviePlaying() || IsRunningDedicatedServer())
	{
		return;
	}

	
	// @note following code works fine for single player but causes a crash in PIE multiplayer tests. Temporarily disabled.
	if (!IsValid(LoadingScreenWidget))
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
	}

	if (IsValid(LoadingScreenWidget))
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();


		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	/*
	if (GetMoviePlayer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	*/
}

void UEODGameInstance::OnPostLoadMap(UWorld* WorldObj)
{
	/*
	check(WorldObj);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SkillTreeManager = WorldObj->SpawnActor <APlayerSkillTreeManager>(PlayerSkillTreeManagerClass.Get(), SpawnInfo);

	check(SkillTreeManager);
	SkillTreeManager->CreateSkillTreeWidget(this);
	*/
}