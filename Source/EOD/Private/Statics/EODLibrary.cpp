// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLibrary.h"
#include "GameSingleton.h"

#include "Engine/Engine.h"

UGameSingleton* UEODLibrary::GetGameSingleton()
{
	UGameSingleton* GameSingleton = GEngine ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	return GameSingleton;
}

UGameUserSettings* UEODLibrary::GetGameUserSettings()
{
	return GEngine ? GEngine->GameUserSettings : nullptr;
}
