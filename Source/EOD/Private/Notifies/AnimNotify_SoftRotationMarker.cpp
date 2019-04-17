// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SoftRotationMarker.h"
#include "PlayerCharacter.h"
#include "EODCharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SoftRotationMarker::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* PlayerCharacter = MeshComp ? Cast<APlayerCharacter>(MeshComp->GetOwner()) : nullptr;
	UEODCharacterMovementComponent* MoveComp = PlayerCharacter ? Cast<UEODCharacterMovementComponent>(PlayerCharacter->GetMovementComponent()) : nullptr;
	if (MoveComp)
	{
		MoveComp->SetDesiredCustomRotationYaw(PlayerCharacter->GetControllerRotationYaw());
	}
}
