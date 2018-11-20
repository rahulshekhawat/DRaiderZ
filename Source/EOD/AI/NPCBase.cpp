// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "NPCBase.h"

#include "Camera/CameraComponent.h"

ANPCBase::ANPCBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractionCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	InteractionCamera->SetupAttachment(RootComponent);
	InteractionCamera->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
	InteractionCamera->AddLocalOffset(FVector(-400.f, 0.f, 0.f));
	
}

void ANPCBase::OnInteract_Implementation(const AEODCharacterBase* Character)
{
}

void ANPCBase::EnableCustomDepth_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ANPCBase::DisableCustomDepth_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}
