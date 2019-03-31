// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_NormalAttack.h"
#include "EODPreprocessors.h"
#include "HumanCharacter.h"
#include "EODCharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"


#if EOD_TEST_CODE_ENABLED
#include "PlayerCharacter.h"
#endif // EOD_TEST_CODE_ENABLED

void UAnimNotify_NormalAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#if EOD_TEST_CODE_ENABLED
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnNormalAttackSectionStart(SectionName);

		// Set off smooth rotation
		FRotator DesiredRotation = FRotator(0.f, PlayerCharacter->GetControllerRotationYaw(), 0.f);
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(PlayerCharacter->GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->SetDesiredCustomRotation(DesiredRotation);
		}
	}
#endif // EOD_TEST_CODE_ENABLED

	AHumanCharacter* HumanCharacter = Cast<AHumanCharacter>(MeshComp->GetOwner());
	if (!HumanCharacter)
	{
		return;
	}

	// @todo Human character normal attack section change and set off smooth rotation

}
