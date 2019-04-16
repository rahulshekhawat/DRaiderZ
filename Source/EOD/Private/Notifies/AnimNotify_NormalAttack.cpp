// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_NormalAttack.h"
#include "HumanCharacter.h"
#include "EODCharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_NormalAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AHumanCharacter* HumanCharacter = Cast<AHumanCharacter>(MeshComp->GetOwner());
	if (HumanCharacter)
	{
		HumanCharacter->OnNormalAttackSectionStart(SectionName);

		// Set off smooth rotation
		FRotator DesiredRotation = FRotator(0.f, HumanCharacter->GetControllerRotationYaw(), 0.f);
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(HumanCharacter->GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->SetDesiredCustomRotation(DesiredRotation);
		}
	}
}
