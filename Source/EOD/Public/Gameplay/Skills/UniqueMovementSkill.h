// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkillBase.h"
#include "UniqueMovementSkill.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UUniqueMovementSkill : public UActiveSkillBase
{
	GENERATED_BODY()

public:

	UUniqueMovementSkill(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	virtual bool CanTriggerSkill() const;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill();


	
};
