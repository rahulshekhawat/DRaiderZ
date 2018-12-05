// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SoftRotationMarker.h"
#include "EOD/Player/PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SoftRotationMarker::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->SetOffSmoothRotation(PlayerCharacter->GetPlayerControlRotationYaw());

}
