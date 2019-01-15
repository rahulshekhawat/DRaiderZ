// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODGameInstance.h"
#include "EOD/SaveSystem/MetaSaveGame.h"
#include "EOD/SaveSystem/PlayerSaveGame.h"
#include "EOD/Statics/EODGlobalNames.h"

#include "Kismet/GameplayStatics.h"

const int32 UEODGameInstance::PlayerIndex(0);
const FString UEODGameInstance::MetaSaveSlotName(TEXT("MetaSaveSlot"));

UEODGameInstance::UEODGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

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
		}
	}

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
