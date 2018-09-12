// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(VisibleAnywhere, Category = PlayerSkills)
	TArray<FName> UnlockedSkills;
	
	
};
