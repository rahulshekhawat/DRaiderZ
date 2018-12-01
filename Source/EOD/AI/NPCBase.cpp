// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "NPCBase.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ANPCBase::ANPCBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractionCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	InteractionCamera->SetupAttachment(RootComponent);
	InteractionCamera->AddLocalRotation(FRotator(0.f, 180.f, 0.f));
	// InteractionCamera->AddLocalOffset(FVector(-400.f, 0.f, 0.f));
	InteractionCamera->SetWorldLocation(FVector(300.f, 0.f, -25.f));
	
}

void ANPCBase::OnInteract_Implementation(const AEODCharacterBase * Character, UUserWidget * DialogueWidget)
{
	UKismetSystemLibrary::PrintString(this, FString("On interaction called from interface of ANPCBase"));
}

void ANPCBase::EnableCustomDepth_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ANPCBase::DisableCustomDepth_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}
