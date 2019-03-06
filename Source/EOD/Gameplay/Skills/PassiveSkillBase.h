// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/GameplaySkillBase.h"
#include "PassiveSkillBase.generated.h"

/**
 * Base class for all passive skills
 */
UCLASS()
class EOD_API UPassiveSkillBase : public UGameplaySkillBase
{
	GENERATED_BODY()

public:
	UPassiveSkillBase(const FObjectInitializer& ObjectInitializer);


};
