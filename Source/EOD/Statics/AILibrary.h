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
	static const FName BBKey_bHasEnemyTarget;

	static const FName BBKey_TargetEnemy;

	static const FName BBKey_AggroActivationRadius;

	static const FName BBKey_AggroAreaRadius;

	static const FName BBKey_MaxEnemyChaseRadius;

	static const FName BBKey_WanderRadius;

	static const FName BBKey_SpawnLocation;

	static const FName BBKey_MostWeightedSkillID;
	//~ End blackboard key names
	
	
	
};
