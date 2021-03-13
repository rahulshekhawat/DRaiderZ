// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Core/RaiderzPlayerController.h"
#include "EODCharacterBase.h"

ARaiderzPlayerController::ARaiderzPlayerController()
{
}

void ARaiderzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	
	//~ Mouse Input
	InputComponent->BindAxis("Turn", this, &ARaiderzPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &ARaiderzPlayerController::AddPitchInput);

	//~ Movement Input
	InputComponent->BindAxis("MoveForward",	this, &ARaiderzPlayerController::MovePawnForward);
	InputComponent->BindAxis("MoveRight", this,	&ARaiderzPlayerController::MovePawnRight);
	
}

void ARaiderzPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	EODCharacter = InPawn ? Cast<AEODCharacterBase>(InPawn) : nullptr;
}

void ARaiderzPlayerController::MovePawnForward(const float Value)
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->MoveForward(Value);
	}
}

void ARaiderzPlayerController::MovePawnRight(const float Value)
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->MoveRight(Value);
	}
}
