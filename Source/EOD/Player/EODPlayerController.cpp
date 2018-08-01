// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODPlayerController.h"

AEODPlayerController::AEODPlayerController(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
}

void AEODPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Mouse Input
	InputComponent->BindAxis("Turn", this, &AEODPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AEODPlayerController::AddPitchInput);

}




