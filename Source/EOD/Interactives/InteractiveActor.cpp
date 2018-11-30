// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveActor.h"

#include "Kismet/KismetSystemLibrary.h"

AInteractiveActor::AInteractiveActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActor::OnInteract_Implementation(const AEODCharacterBase * Character, UUserWidget * DialogueWidget)
{
	UKismetSystemLibrary::PrintString(this, FString("On interaction called from interface of AInteractable"));
}
