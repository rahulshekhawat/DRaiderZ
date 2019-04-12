// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "GameFramework/SaveGame.h"
#include "MetaSaveGame.generated.h"

USTRUCT(BlueprintType)
struct EOD_API FMetaSaveGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	FString CharacterName;

	UPROPERTY()
	FDateTime LastSaveTime;

	UPROPERTY()
	int32 PlayerIndex;
};


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
	TArray<FMetaSaveGameData> SaveSlotMetaDataList;

	UPROPERTY()
	FString LastUsedSlotName;


};
