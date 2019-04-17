// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveStaticMesh.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

const FName AInteractiveStaticMesh::PrimaryMeshComponentName(TEXT("Primary Mesh"));
const FName AInteractiveStaticMesh::InteractionCameraComponentName(TEXT("Camera"));

AInteractiveStaticMesh::AInteractiveStaticMesh(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, AInteractiveStaticMesh::PrimaryMeshComponentName);
	if (PrimaryMesh)
	{
		PrimaryMesh->SetMobility(EComponentMobility::Static);
		RootComponent = PrimaryMesh;
	}

	InteractionCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, AInteractiveStaticMesh::InteractionCameraComponentName);
	if (InteractionCamera)
	{
		InteractionCamera->SetupAttachment(RootComponent);
		InteractionCamera->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
		InteractionCamera->SetWorldLocation(FVector(400.f, 0.f, 150.f));
	}
}

void AInteractiveStaticMesh::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveStaticMesh::EnableCustomDepth_Implementation()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->SetRenderCustomDepth(true);
	}
}

void AInteractiveStaticMesh::DisableCustomDepth_Implementation()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->SetRenderCustomDepth(false);
	}
}
