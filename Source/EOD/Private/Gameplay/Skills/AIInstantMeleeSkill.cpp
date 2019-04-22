// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AIInstantMeleeSkill.h"
#include "EODCharacterBase.h"

UAIInstantMeleeSkill::UAIInstantMeleeSkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAIInstantMeleeSkill::TriggerSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (!Instigator)
	{
		return;
	}

	bool bIsLocalController = Instigator->Controller && Instigator->Controller->IsLocalController();
	if (bIsLocalController)
	{
		Instigator->SetCharacterStateAllowsMovement(false);
		Instigator->SetCharacterStateAllowsRotation(false);

		FCharacterStateInfo StateInfo(ECharacterState::UsingActiveSkill, SkillIndex);
		StateInfo.NewReplicationIndex = Instigator->CharacterStateInfo.NewReplicationIndex + 1;
		Instigator->CharacterStateInfo = StateInfo;
	}

	if (SkillMontage)
	{
		SkillDuration = Instigator->PlayAnimMontage(SkillMontage, 1.f, AnimationStartSectionName);
		float ActualSkillDuration;

		if (SkillMontage->BlendOutTriggerTime >= 0.f)
		{
			ActualSkillDuration = SkillDuration;
		}
		else
		{
			ActualSkillDuration = SkillDuration - SkillMontage->BlendOut.GetBlendTime();
		}

		UWorld* World = Instigator->GetWorld();
		check(World);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UAIInstantMeleeSkill::FinishSkill);
		World->GetTimerManager().SetTimer(SkillTimerHandle, TimerDelegate, ActualSkillDuration, false);

		Instigator->OnSkillActivated(SkillIndex, SkillGroup, this);
	}
	else
	{
		Instigator->ResetState();
	}
}
