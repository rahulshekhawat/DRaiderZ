// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "GameSingleton.h"

UGameSingleton::UGameSingleton(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	PlayerAnimationReferencesDataTable = nullptr;
	WeaponDataTable = nullptr;
}
