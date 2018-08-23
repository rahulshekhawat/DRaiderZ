// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AILibrary.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAILibrary : public UObject
{
	GENERATED_BODY()
	
public:

	UAILibrary(const FObjectInitializer& ObjectInitializer);

	//~ Begin blackboard key names
	static FName BBKey_bHasEnemyTarget;

	static FName BBKey_TargetEnemy;

	static FName BBKey_AggroActivationRadius;

	static FName BBKey_AggroAreaRadius;

	static FName BBKey_MaxEnemyChaseRadius;

	static FName BBKey_WanderRadius;

	static FName BBKey_SpawnLocation;
	//~ End blackboard key names
	
	
	
};
