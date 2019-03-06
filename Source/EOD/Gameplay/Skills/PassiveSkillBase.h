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

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Activate this skill */
	virtual void ActivateSkill() override;

	/** Cancel this skill */
	virtual void CancelSkill() override;

	/** Deactivate this skill */
	virtual void EndSkill() override;

protected:

	/** Returns true if this activity can be activated */
	virtual bool CanActivateSkill() const override;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const override;


};
