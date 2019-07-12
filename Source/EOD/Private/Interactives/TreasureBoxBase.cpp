// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "TreasureBoxBase.h"
#include "EODCharacterBase.h"

#include "Components/SkeletalMeshComponent.h"

ATreasureBoxBase::ATreasureBoxBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSpawnByDefault = true;

	if (PrimaryMesh)
	{
		PrimaryMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	}
}

void ATreasureBoxBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bSpawnByDefault)
	{
		PrimaryMesh->PlayAnimation(VisibleIdleAnimation, false);
	}
	else
	{
		PrimaryMesh->PlayAnimation(HiddenIdleAnimation, false);
	}
}

void ATreasureBoxBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATreasureBoxBase::OnInteract_Implementation(AEODCharacterBase* Character)
{
	// Ignore interaction request if it's not the local player interacting with us
	if (!Character->Controller && !Character->Controller->IsLocalPlayerController())
	{
		return;
	}

	OpenBox();

	//~ @todo check for loot, play player loot animation if there's loot, etc.

}

void ATreasureBoxBase::SpawnBox()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->PlayAnimation(ChestDropAnimation, false);
	}
}

void ATreasureBoxBase::OpenBox()
{
	// If the box is already open
	if (bOpen)
	{
		return;
	}

	if (PrimaryMesh)
	{
		PrimaryMesh->PlayAnimation(ChestOpenAnimation, false);
	}
	bOpen = true;
}
