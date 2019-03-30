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

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner) override;

	/** Activate this skill */
	virtual void ActivateSkill() override;

	/** Cancel this skill */
	virtual void CancelSkill() override;

	/** Deactivate this skill */
	virtual void EndSkill() override;

	/** Returns true if this activity can be activated */
	virtual bool CanActivateSkill() const override;


};
