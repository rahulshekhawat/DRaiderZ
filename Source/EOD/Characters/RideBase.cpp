// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "RideBase.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"

#include "UnrealNetwork.h"

FName ARideBase::MountSocketName(TEXT("ride_DummySocket"));

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

void ARideBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARideBase, MountedCharacter);

}

void ARideBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARideBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ARideBase::MountCharacter(AEODCharacterBase* Character)
{
	MountedCharacter = Character;
	if (IsValid(MountedCharacter))
	{
		MountedCharacter->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, MountSocketName);
		MountedCharacter->OnMountingRide(this);
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
	// If the ride can't fly, just use the default rotation method 
	if (!bCanFly)
	{
		Super::UpdateRotation(DeltaTime);
		return;
	}

	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (!MoveComp)
	{
		return;
	}

	FRotator NewDesiredRotation = FRotator::ZeroRotator;
	FRotator ControllerRotation = GetControlRotation();
	FRotator ActorRotation = GetActorRotation();

	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		NewDesiredRotation.Yaw = ActorRotation.Yaw;
	}
	else
	{
		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0)
			{
				NewDesiredRotation.Yaw = FMath::UnwindDegrees(ControllerRotation.Yaw + 90.f);
			}
			else if (RightAxisValue < 0)
			{
				NewDesiredRotation.Yaw = FMath::UnwindDegrees(ControllerRotation.Yaw - 90.f);
			}
		}
		else
		{
			NewDesiredRotation.Pitch = FMath::UnwindDegrees(ControllerRotation.Pitch);
			float DeltaAngle = 0;
			if (ForwardAxisValue > 0)
			{
				// NewDesiredRotation.Pitch = FMath::UnwindDegrees(ControllerRotation.Pitch - 5.f);
				DeltaAngle = FMath::RadiansToDegrees(FMath::Atan2(RightAxisValue, ForwardAxisValue));
			}
			else if (ForwardAxisValue < 0)
			{
				// NewDesiredRotation.Pitch = FMath::UnwindDegrees(ControllerRotation.Pitch + 5.f);
				DeltaAngle = FMath::RadiansToDegrees(FMath::Atan2(-RightAxisValue, -ForwardAxisValue));
			}
			NewDesiredRotation.Yaw = FMath::UnwindDegrees(ControllerRotation.Yaw + DeltaAngle);

		}

		if (RightAxisValue > 0)
		{
			// NewDesiredRotation.Roll = 15;
		}
		else if (RightAxisValue < 0)
		{
			// NewDesiredRotation.Roll = -15;
		}
	}

	MoveComp->SetDesiredCustomRotation(NewDesiredRotation);
}

void ARideBase::OnRep_MountedCharacter(AEODCharacterBase* OldCharacter)
{
	if (IsValid(MountedCharacter))
	{
		MountedCharacter->OnMountingRide(this);
	}
}
