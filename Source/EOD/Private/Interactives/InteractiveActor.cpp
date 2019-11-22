// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractiveActor.h"
#include "EODPlayerController.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

AInteractiveActor::AInteractiveActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

}

void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();	
}

void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveActor::EnableCustomDepth_Implementation()
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponentsArray;
	this->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponentsArray);
	for (USkeletalMeshComponent* SMComp : SkeletalMeshComponentsArray)
	{
		if (SMComp)
		{
			SMComp->SetRenderCustomDepth(true);
		}
	}

	TArray<UStaticMeshComponent*> StaticMeshComponentsArray;
	this->GetComponents<UStaticMeshComponent>(StaticMeshComponentsArray);
	for (UStaticMeshComponent* SMComp : StaticMeshComponentsArray)
	{
		if (SMComp)
		{
			SMComp->SetRenderCustomDepth(true);
		}
	}
}

void AInteractiveActor::DisableCustomDepth_Implementation()
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponentsArray;
	this->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponentsArray);
	for (USkeletalMeshComponent* SMComp : SkeletalMeshComponentsArray)
	{
		if (SMComp)
		{
			SMComp->SetRenderCustomDepth(false);
		}
	}

	TArray<UStaticMeshComponent*> StaticMeshComponentsArray;
	this->GetComponents<UStaticMeshComponent>(StaticMeshComponentsArray);
	for (UStaticMeshComponent* SMComp : StaticMeshComponentsArray)
	{
		if (SMComp)
		{
			SMComp->SetRenderCustomDepth(false);
		}
	}
}

void AInteractiveActor::OnGainFocus_Implementation(AEODCharacterBase* Character)
{
	AEODPlayerController* PC = Character ? Cast<AEODPlayerController>(Character->Controller) : nullptr;
	if (PC)
	{
		PC->RegisterPopupWidget(this, TEXT(""), TEXT(""), nullptr);
	}
}

void AInteractiveActor::OnLoseFocus_Implementation(AEODCharacterBase* Character)
{
	AEODPlayerController* PC = Character ? Cast<AEODPlayerController>(Character->Controller) : nullptr;
	if (PC)
	{
		PC->UnregisterPopupWidget(this);
	}
}
