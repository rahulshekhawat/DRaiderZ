// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "GameFramework/SaveGame.h"
#include "EODSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UEODSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	/** A map of skill ID to skill bar slot index */
	UPROPERTY()
	TMap<FString, int32> SkillBarLayout;

	/** A map of skill to it's state */
	UPROPERTY()
	TMap<FString, FSkillState> SkillToStateMap;
	
};
