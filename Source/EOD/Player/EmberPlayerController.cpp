// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EmberPlayerController.h"

AEmberPlayerController::AEmberPlayerController(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
}

void AEmberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Mouse Input
	InputComponent->BindAxis("Turn", this, &AEmberPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AEmberPlayerController::AddPitchInput);

}
