// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "RideBase.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"


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

void ARideBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetController() && GetController()->IsLocalPlayerController())
	{
		// UpdateRotation(DeltaTime);
	}
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

void ARideBase::UpdateRotation(float DeltaTime)
{
	if (!bCanFly)
	{
		Super::UpdateRotation(DeltaTime);
	}

	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (!MoveComp)
	{
		return;
	}

	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		
		// MoveComp->SetDesiredCustomRotationYaw();


	}
}
