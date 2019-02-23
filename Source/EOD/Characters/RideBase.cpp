// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "RideBase.h"

#include "GameFramework/CharacterMovementComponent.h"


ARideBase::ARideBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	USpringArmComponent* SpringArm = GetCameraBoomComponent();
	if (SpringArm)
	{
		SpringArm->AddLocalOffset(FVector(0.f, 0.f, 40.f));
	}
}

void ARideBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARideBase::BeginPlay()
{
	Super::BeginPlay();


}

void ARideBase::Restart()
{
	Super::Restart();

	//~ @note Setting the movement mode in ::Restart() because setting the movement mode to flying in ::BeginPlay() or ::PostInitializeComponents() just resets it back to MOVE_Walking
	if (bCanFly)
	{
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (MoveComp)
		{
			MoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}

}

void ARideBase::MoveForward(const float Value)
{
	ForwardAxisValue = Value;

	if (Value != 0 && CanMove())
	{
		FVector Direction = GetControlRotation().Vector();
		AddMovementInput(Direction , Value);
	}
}

void ARideBase::MoveRight(const float Value)
{
	RightAxisValue = Value;

	if (Value != 0 && CanMove())
	{
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
