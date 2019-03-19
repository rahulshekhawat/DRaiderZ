// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

USTRUCT(BlueprintType)
struct EOD_API FSkillTreeSlotSaveData
{
	GENERATED_USTRUCT_BODY()

public:

	/** The slot upgrade level that player has unlocked  */
	UPROPERTY()
	int32 CurrentUpgrade;

	FSkillTreeSlotSaveData()
	{
		CurrentUpgrade = 0;
	}
};


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
	UPROPERTY(BlueprintReadOnly, Category = "Player Save Game")
	ECharacterGender CharacterGender;

	/** Skill button index to skill group map for skill bar  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Save Game")
	TMap<int32, FString> SkillBarLayout;

	/** A map of skill to it's state */
	UPROPERTY(BlueprintReadOnly, Category = "Player Save Game")
	TMap<FString, FSkillState> SGToSSMap;

	/** A map of skill group to it's skill tree slot info */
	UPROPERTY(BlueprintReadOnly, Category = "Player Save Game")
	TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData;

};
