// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "InstantMeleeSkill_NoSmoothRot.h"
#include "EODCharacterBase.h"
#include "EODCharacterMovementComponent.h"

UInstantMeleeSkill_NoSmoothRot::UInstantMeleeSkill_NoSmoothRot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInstantMeleeSkill_NoSmoothRot::ApplyRotation()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
	if (MoveComp)
	{
		float DesiredRotationYaw = Instigator->GetControllerRotationYaw();
		Instigator->SetCharacterRotationYaw(DesiredRotationYaw);
		MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
	}
}
