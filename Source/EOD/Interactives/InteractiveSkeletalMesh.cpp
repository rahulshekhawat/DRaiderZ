// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveSkeletalMesh.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

AInteractiveSkeletalMesh::AInteractiveSkeletalMesh(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RootMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, FName("Root Mesh"));
	RootComponent = RootMesh;

	InteractionCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	InteractionCamera->SetupAttachment(RootComponent);
	InteractionCamera->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
	// InteractionCamera->AddLocalOffset(FVector(-400.f, 0.f, 0.f));
	InteractionCamera->SetWorldLocation(FVector(400.f, 0.f, 150.f));
}

void AInteractiveSkeletalMesh::EnableCustomDepth_Implementation()
{
	RootMesh->SetRenderCustomDepth(true);
}

void AInteractiveSkeletalMesh::DisableCustomDepth_Implementation()
{
	RootMesh->SetRenderCustomDepth(false);
}
