// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/ActiveSkillBase.h"
#include "EscapeSkillBase.generated.h"

/**
 * Base class for all "escape" skills
 * A character can use escape skill anytime it wishes to even if it's under a crowd control effect
 */
UCLASS()
class EOD_API UEscapeSkillBase : public UActiveSkillBase
{
	GENERATED_BODY()
	
public:
	UEscapeSkillBase(const FObjectInitializer& ObjectInitializer);



};
