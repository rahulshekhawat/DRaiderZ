// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPlayerSaveGame(const FObjectInitializer& ObjectInitializer);

};
