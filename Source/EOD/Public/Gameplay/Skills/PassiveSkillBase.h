// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplaySkillBase.h"
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

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Cancel this skill */
	virtual void CancelSkill() override;

protected:

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const override;


};
