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

	virtual bool CanTriggerSkill() const;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

	virtual bool CanReleaseSkill() const;

	/** Release this skill, i.e., if this skill is chargeable then activate this skill, otherwise it does nothing */
	virtual void ReleaseSkill(float ChargeDuration) override;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const;

	/** Cancel this skill */
	virtual void CancelSkill() override;


};
