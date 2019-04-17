// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveSkeletalMesh.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

const FName AInteractiveSkeletalMesh::PrimaryMeshComponentName(TEXT("Primary Mesh"));
const FName AInteractiveSkeletalMesh::InteractionCameraComponentName(TEXT("Camera"));

AInteractiveSkeletalMesh::AInteractiveSkeletalMesh(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, AInteractiveSkeletalMesh::PrimaryMeshComponentName);
	if (PrimaryMesh)
	{
		RootComponent = PrimaryMesh;
	}

	InteractionCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, AInteractiveSkeletalMesh::InteractionCameraComponentName);
	if (InteractionCamera)
	{
		InteractionCamera->SetupAttachment(RootComponent);
		InteractionCamera->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
		InteractionCamera->SetWorldLocation(FVector(400.f, 0.f, 150.f));
	}
}

void AInteractiveSkeletalMesh::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveSkeletalMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveSkeletalMesh::EnableCustomDepth_Implementation()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->SetRenderCustomDepth(true);
	}
}

void AInteractiveSkeletalMesh::DisableCustomDepth_Implementation()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->SetRenderCustomDepth(false);
	}
}
