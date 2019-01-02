// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MetaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UMetaSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMetaSaveGame(const FObjectInitializer& ObjectInitializer);

	/** A list of all the save game profiles saved on the system */
	UPROPERTY()
	TArray<FString> SavedProfilesList;


};
