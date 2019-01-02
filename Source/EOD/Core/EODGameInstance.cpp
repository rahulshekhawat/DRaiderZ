// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODGameInstance.h"
#include "EOD/SaveSystem/MetaSaveGame.h"
#include "EOD/Statics/EODGlobalNames.h"

#include "Kismet/GameplayStatics.h"

const int32 UEODGameInstance::UserIndex(0);
const FString UEODGameInstance::DefaultMetaSaveSlotName(TEXT("DefaultSlot"));

UEODGameInstance::UEODGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEODGameInstance::Init()
{
	Super::Init();

	if (UGameplayStatics::DoesSaveGameExist(UEODGameInstance::DefaultMetaSaveSlotName, UEODGameInstance::UserIndex))
	{
		MetaSaveGameObject = Cast<UMetaSaveGame>(UGameplayStatics::LoadGameFromSlot(UEODGameInstance::DefaultMetaSaveSlotName, UEODGameInstance::UserIndex));
	}
	else
	{
		MetaSaveGameObject = Cast<UMetaSaveGame>(UGameplayStatics::CreateSaveGameObject(UMetaSaveGame::StaticClass()));
		if (IsValid(MetaSaveGameObject))
		{
			UGameplayStatics::SaveGameToSlot(MetaSaveGameObject, UEODGameInstance::DefaultMetaSaveSlotName, UEODGameInstance::UserIndex);
		}
	}

}
