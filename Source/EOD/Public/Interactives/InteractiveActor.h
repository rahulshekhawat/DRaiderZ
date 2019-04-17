// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"

#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

UCLASS()
class EOD_API AInteractiveActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AInteractiveActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Interaction Interface
	// --------------------------------------

	virtual void OnInteract_Implementation(AEODCharacterBase* Character) override;

};
