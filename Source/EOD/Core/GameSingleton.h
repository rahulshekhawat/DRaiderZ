// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "GameSingleton.generated.h"

/**
 * EOD's singleton class
 */
UCLASS(Blueprintable)
class EOD_API UGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:

	UGameSingleton(const FObjectInitializer& ObjectInitializer);

	/** A singleton instance of FStreamableManager */
	FStreamableManager StreamableManager;
	
	/** Data table containing player animation references */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* PlayerAnimationReferencesDataTable;
	
	/** Data table containing weapons info */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* WeaponsDataTable;

	/** Data table containing player skills */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* PlayerSkillsDataTable;

	UPROPERTY(BlueprintReadOnly, Category = SaveGame)
	FString CurrentSaveSlotName;

	UPROPERTY(BlueprintReadOnly, Category = SaveGame)
	int32 UserIndex;



};
