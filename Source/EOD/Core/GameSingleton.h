// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnimationsReferences)
	UDataTable* PlayerAnimationReferencesDataTable;
	
	/** Reference to data table containing weapons data */
	UPROPERTY(EditDefaultsOnly, Category = Weapons)
	UDataTable* WeaponDataTable;

};
