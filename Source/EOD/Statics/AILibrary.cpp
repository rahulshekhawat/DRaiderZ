// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AILibrary.h"

FName UAILibrary::BBKey_bHasEnemyTarget				= FName("bHasEnemyTarget");
FName UAILibrary::BBKey_TargetEnemy					= FName("TargetEnemy");
FName UAILibrary::BBKey_AggroActivationRadius		= FName("AggroActivationRadius");
FName UAILibrary::BBKey_AggroAreaRadius				= FName("AggroAreaRadius");
FName UAILibrary::BBKey_MaxEnemyChaseRadius			= FName("MaxEnemyChaseRadius");
FName UAILibrary::BBKey_WanderRadius				= FName("WanderRadius");
FName UAILibrary::BBKey_SpawnLocation				= FName("SpawnLocation");

UAILibrary::UAILibrary(const FObjectInitializer & ObjectInitializer)
{
}
