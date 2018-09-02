// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "Core/GameSingleton.h"

#include "GameFramework/CharacterMovementComponent.h"

AAICharacterBase::AAICharacterBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	// Mob characters don't have strafe animations and so they must be rotated in the direction of their movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AAICharacterBase::SetInCombat(bool bValue)
{
	bInCombat = bValue;
	UpdateMaxWalkSpeed();
}

void AAICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UCharacterLibrary::GetAllAICharacterSkills(InGameName, DataTable_Skills, Skills);

}

void AAICharacterBase::OnMeleeCollision(UAnimSequenceBase * Animation, TArray<FHitResult>& HitResults, bool bHit)
{
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetInCombat(false);


}

void AAICharacterBase::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;	
}
