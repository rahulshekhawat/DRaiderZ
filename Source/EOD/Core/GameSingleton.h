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
	
	/** Reference to data table containing player animation references data */
	UPROPERTY(EditDefaultsOnly, Category = DataTable)
	UDataTable* PlayerAnimationReferencesDataTable;
	
	/** Reference to data table containing weapons data */
	UPROPERTY(EditDefaultsOnly, Category = DataTable)
	UDataTable* WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, Category = DataTable)
	UDataTable* PlayerSkillsTable;

	// UPROPERTY(EditDefaultsOnly, Category = UI)
	// TSubclassOf<UUserWidget> FloatingDamageNumberWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = SaveGame)
	FString CurrentSaveSlotName;

	UPROPERTY(BlueprintReadOnly, Category = SaveGame)
	int32 UserIndex;



};
