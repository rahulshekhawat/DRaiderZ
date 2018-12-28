// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_NormalAttack.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Characters/HumanCharacter.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"


#if EOD_TEST_CODE_ENABLED
#include "EOD/Characters/PlayerCharacter.h"
#endif // EOD_TEST_CODE_ENABLED

void UAnimNotify_NormalAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#if EOD_TEST_CODE_ENABLED
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnNormalAttackSectionStart(SectionName);

		/*
		// Set off smooth rotation
		float DesiredYaw = PlayerCharacter->GetControllerRotationYaw();
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(PlayerCharacter->GetCharacterMovement());
		if (MoveComp->GetDesiredCustomRotationYaw() != DesiredYaw)
		{
			MoveComp->SetDesiredCustomRotationYaw(DesiredYaw);
		}
		*/
		// Following line of code removed because this method of rotation is now deprecated
		// PlayerCharacter->SetOffSmoothRotation(PlayerCharacter->GetControllerRotationYaw());
	}
#endif // EOD_TEST_CODE_ENABLED

	AHumanCharacter* HumanCharacter = Cast<AHumanCharacter>(MeshComp->GetOwner());
	if (!HumanCharacter)
	{
		return;
	}

	// @todo Human character normal attack section change and set off smooth rotation

}
