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

	UPROPERTY(BlueprintReadOnly, Category = "Save Game Meta Data")
	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly, Category = "Save Game Meta Data")
	FString CharacterName;

	UPROPERTY(BlueprintReadOnly, Category = "Save Game Meta Data")
	FDateTime LastSaveTime;

	UPROPERTY(BlueprintReadOnly, Category = "Save Game Meta Data")
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
	UPROPERTY(BlueprintReadOnly, Category = "Meta Save Game")
	TArray<FMetaSaveGameData> SaveSlotMetaDataList;

	UPROPERTY(BlueprintReadOnly, Category = "Meta Save Game")
	FString LastUsedSlotName;


};
