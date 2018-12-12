// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveStaticMesh.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

AInteractiveStaticMesh::AInteractiveStaticMesh(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RootMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("Root Mesh"));
	RootMesh->SetMobility(EComponentMobility::Static);
	RootComponent = RootMesh;

	InteractionCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	InteractionCamera->SetupAttachment(RootComponent);
	InteractionCamera->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
	// InteractionCamera->AddLocalOffset(FVector(-400.f, 0.f, 0.f));
	InteractionCamera->SetWorldLocation(FVector(400.f, 0.f, 150.f));
}

void AInteractiveStaticMesh::EnableCustomDepth_Implementation()
{
	RootMesh->SetRenderCustomDepth(true);
}

void AInteractiveStaticMesh::DisableCustomDepth_Implementation()
{
	RootMesh->SetRenderCustomDepth(false);
}
