// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/GameplaySkillBase.h"
#include "PlayerSkillBase.generated.h"

/**
 * The base class of skills that can be used by player, added to skill bar, upgraded by player, etc.
 */
UCLASS()
class EOD_API UPlayerSkillBase : public UGameplaySkillBase
{
	GENERATED_BODY()
	
public:

	UPlayerSkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------



};
