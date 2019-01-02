// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"
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

	/** Skill button index to skill group map for skill bar  */
	UPROPERTY(BlueprintReadOnly, Category = "Player Save Game")
	TMap<int32, FString> SkillBarLayout;

	/** A map of skill to it's state */
	UPROPERTY(BlueprintReadOnly, Category = "Player Save Game")
	TMap<FString, FSkillState> SGToSSMap;

};
