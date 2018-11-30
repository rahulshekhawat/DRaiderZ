// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLibrary.h"
#include "Core/GameSingleton.h"

#include "Engine/Engine.h"

UGameSingleton* UEODLibrary::GetGameSingleton()
{
	UGameSingleton* GameSingleton = nullptr;
	GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	return GameSingleton;
}
