// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveActor.h"

AInteractiveActor::AInteractiveActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();	
}

void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActor::OnInteract_Implementation(AEODCharacterBase* Character)
{
}
