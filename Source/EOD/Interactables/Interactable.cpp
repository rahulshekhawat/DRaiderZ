// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "Interactable.h"

// Sets default values
AInteractable::AInteractable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::OnInteract_Implementation(const AEODCharacterBase* Character)
{
}

void AInteractable::EnableCustomDepth_Implementation()
{
}

void AInteractable::DisableCustomDepth_Implementation()
{
}

