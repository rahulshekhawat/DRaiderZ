// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/Components/EODCharacterMovementComponent.h"
#include "EOD/Core/EODPreprocessors.h"

#include "UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

UEODCharacterMovementComponent::UEODCharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RotationRate = FRotator(0.f, 600.f, 0.f);
}

void UEODCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UEODCharacterMovementComponent, DesiredCustomRotationYaw);
	DOREPLIFETIME_CONDITION(UEODCharacterMovementComponent, DesiredCustomRotationYaw, COND_SkipOwner);
}

void UEODCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	const float AngleTolerance = 1e-3f;
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	CurrentRotation.DiagnosticCheckNaN(TEXT("EODCharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

	if (!(bOrientRotationToMovement || bUseControllerDesiredRotation || !FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredCustomRotationYaw, AngleTolerance)))
	{
		return;
	}

	if (!HasValidData() || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		return;
	}

	FRotator DeltaRot = GetDeltaRotation(DeltaTime);
	DeltaRot.DiagnosticCheckNaN(TEXT("EODCharacterMovementComponent::PhysicsRotation(): GetDeltaRotation"));

	FRotator DesiredRotation = CurrentRotation;
	if (bOrientRotationToMovement)
	{
		DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);
	}
	else if (CharacterOwner->Controller && bUseControllerDesiredRotation)
	{
		DesiredRotation = CharacterOwner->Controller->GetDesiredRotation();
	}
	else if(!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredCustomRotationYaw, AngleTolerance))
	{
		DesiredRotation.Yaw = DesiredCustomRotationYaw;
	}
	else
	{
		return;
	}

	if (ShouldRemainVertical())
	{
		DesiredRotation.Pitch = 0.f;
		DesiredRotation.Yaw = FRotator::NormalizeAxis(DesiredRotation.Yaw);
		DesiredRotation.Roll = 0.f;
	}
	else
	{
		DesiredRotation.Normalize();
	}

	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// PITCH
		if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
		{
			DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
		}

		// YAW
		if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
		{
			DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
		}

		// ROLL
		if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
		{
			DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
		}

		// Set the new rotation.
		DesiredRotation.DiagnosticCheckNaN(TEXT("EODCharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}
}

void UEODCharacterMovementComponent::DoInstantRotation(float InstantRotationYaw)
{
	const float AngleTolerance = 1e-3f;
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	CurrentRotation.DiagnosticCheckNaN(TEXT("EODCharacterMovementComponent::PhysicsRotation(): CurrentRotation"));
	FRotator DesiredRotation = CurrentRotation;
	DesiredRotation.Yaw = FRotator::NormalizeAxis(InstantRotationYaw);

	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		DesiredCustomRotationYaw = InstantRotationYaw;
		// SetDesiredCustomRotationYaw(InstantRotationYaw);
		DesiredRotation.DiagnosticCheckNaN(TEXT("EODCharacterMovementComponent::PhysicsRotation(): DesiredRotation"));
		MoveUpdatedComponent(FVector::ZeroVector, DesiredRotation, /*bSweep*/ false);
	}

	if (IsValid(CharacterOwner) && CharacterOwner->Role < ROLE_Authority)
	{
		Server_DoInstantRotation(InstantRotationYaw);
	}
}

void UEODCharacterMovementComponent::Server_SetDesiredCustomRotationYaw_Implementation(float NewRotationYaw)
{
	// SetDesiredCustomRotationYaw(NewRotationYaw);
}

bool UEODCharacterMovementComponent::Server_SetDesiredCustomRotationYaw_Validate(float NewRotationYaw)
{
	return true;
}

void UEODCharacterMovementComponent::Server_DoInstantRotation_Implementation(float InstantRotationYaw)
{
	DoInstantRotation(InstantRotationYaw);
}

bool UEODCharacterMovementComponent::Server_DoInstantRotation_Validate(float InstantRotationYaw)
{
	return true;
}
