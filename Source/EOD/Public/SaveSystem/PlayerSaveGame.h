// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"


/**
 * A save game clas to store state of player's default character
 *
 * @note Player can actually possess any in-game character, but we will save state of only the default character,
 * i.e., the character that the player starts the game with.
 */
UCLASS()
class EOD_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPlayerSaveGame(const FObjectInitializer& ObjectInitializer);

	/** Character gender that player selected during character creation */
	UPROPERTY()
	ECharacterGender CharacterGender;

	UPROPERTY()
	int32 CharacterLevel;

	/** EXP required for player level up */
	UPROPERTY()
	int32 LevelupEXP;

	/** Number of unlocked skill points currently available */
	UPROPERTY()
	int32 UnlockedSkillPoints;

	/** A map of skill group to it's skill tree slot info */
	UPROPERTY()
	TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData;

	/** Information of how players has allocated his skill points */
	UPROPERTY()
	FSkillPointsAllocationInfo SkillPointsAllocationInfo;

	/** A map of all the skills placed on skill bar to their Skill Index */
	UPROPERTY()
	TMap<uint8, uint8> SkillBarMap;

};
