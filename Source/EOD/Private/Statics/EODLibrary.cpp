// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLibrary.h"
#include "GameSingleton.h"

#include "Engine/Engine.h"

UGameSingleton* UEODLibrary::GetGameSingleton()
{
	return GEngine ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
}

UGameUserSettings* UEODLibrary::GetGameUserSettings()
{
	return GEngine ? GEngine->GameUserSettings : nullptr;
}

USoundBase* UEODLibrary::GetRandomSound(const TArray<USoundBase*> Sounds)
{
	USoundBase* Sound = nullptr;

	int32 SoundsNum = Sounds.Num();
	if (SoundsNum == 1)
	{
		Sound = Sounds[0];
	}
	else if (SoundsNum > 1)
	{
		int32 RandSoundIndex = FMath::RandRange(0, SoundsNum - 1);
		Sound = Sounds[RandSoundIndex];
	}

	return Sound;
}
