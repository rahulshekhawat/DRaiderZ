// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerSaveGame.h"

UPlayerSaveGame::UPlayerSaveGame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	CharacterGender(ECharacterGender::Female),
	CharacterLevel(0),
	LevelupEXP(0),
	UnlockedSkillPoints(0)
{
}
