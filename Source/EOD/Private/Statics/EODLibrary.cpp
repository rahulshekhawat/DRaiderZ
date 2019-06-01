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

FGameplayTagMod& FGameplayTagMod::operator=(FGameplayTagMod const& Other)
{
	this->Tags = Other.Tags;
	this->SourceObj = Other.SourceObj;
	return *this;
}

bool FGameplayTagMod::operator==(FGameplayTagMod const& Other) const
{
	return this->Tags == Other.Tags && this->SourceObj == Other.SourceObj;
}

bool FGameplayTagMod::operator!=(FGameplayTagMod const& Other) const
{
	return this->Tags != Other.Tags || this->SourceObj != Other.SourceObj;
}
