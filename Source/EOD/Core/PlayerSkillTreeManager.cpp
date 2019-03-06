// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerSkillTreeManager.h"

APlayerSkillTreeManager::APlayerSkillTreeManager(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);
	SetReplicateMovement(false);

}
