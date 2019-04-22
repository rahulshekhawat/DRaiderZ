// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/AISkillBase.h"
#include "AIInstantMeleeSkill.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAIInstantMeleeSkill : public UAISkillBase
{
	GENERATED_BODY()
	
public:

	UAIInstantMeleeSkill(const FObjectInitializer& ObjectInitializer);

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

protected:

	FTimerHandle SkillTimerHandle;


};
