// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MobCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AMobCharacter::AMobCharacter(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	// Mob characters don't have strafe animations and so they must be rotated in the direction of their movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;


}

void AMobCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetInCombat(false);


}

void AMobCharacter::SetInCombat(bool bValue)
{
	bInCombat = bValue;
	UpdateMaxWalkSpeed();
}

void AMobCharacter::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;
}
