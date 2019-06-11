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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* PlayerAnimationReferencesDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* PlayerSkillsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* WeaponsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* ArmorsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* DialogueWindowsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* DialogueOptionsDataTable;

public:

	/** A singleton instance of FStreamableManager */
	FStreamableManager StreamableManager;


};
