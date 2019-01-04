// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "NPCBase.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ANPCBase::ANPCBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// If camera is attached to spring arm, it may get blocked in ways that is not intended during player interaction.
	if (GetCameraComponent())
	{
		GetCameraComponent()->SetupAttachment(RootComponent);
		GetCameraComponent()->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
		GetCameraComponent()->SetWorldLocation(FVector(300.f, 0.f, -25.f));
	}
}

void ANPCBase::OnInteract_Implementation(AEODCharacterBase* Character)
{
}

void ANPCBase::EnableCustomDepth_Implementation()
{
	if (IsValid(GetMesh()))
	{
		GetMesh()->SetRenderCustomDepth(true);
	}
}

void ANPCBase::DisableCustomDepth_Implementation()
{
	if (IsValid(GetMesh()))
	{
		GetMesh()->SetRenderCustomDepth(false);
	}
}
