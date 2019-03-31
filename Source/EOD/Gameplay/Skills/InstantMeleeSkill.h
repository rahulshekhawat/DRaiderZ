// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/ActiveSkillBase.h"
#include "InstantMeleeSkill.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UInstantMeleeSkill : public UActiveSkillBase
{
	GENERATED_BODY()
	
public:

	UInstantMeleeSkill(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	virtual bool CanTriggerSkill() const override;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const override;

	/** Cancel this skill */
	virtual void CancelSkill() override;

	virtual void FinishSkill() override;

protected:

	FTimerHandle SkillTimerHandle;

};
