// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Core/EODPreprocessors.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EODCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UEODCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UEODCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Perform rotation over deltaTime */
	virtual void PhysicsRotation(float DeltaTime) override;

	//~ @note: Overriding the default definition to fix a bug where Server drops/skips animation notifies.
	virtual void ServerMoveDual_Implementation(float TimeStamp0,
		FVector_NetQuantize10 InAccel0,
		uint8 PendingFlags,
		uint32 View0,
		float TimeStamp,
		FVector_NetQuantize10 InAccel,
		FVector_NetQuantize100 ClientLoc,
		uint8 NewFlags,
		uint8 ClientRoll,
		uint32 View,
		UPrimitiveComponent* ClientMovementBase,
		FName ClientBaseBoneName,
		uint8 ClientMovementMode) override;

	// --------------------------------------
	//  Rotation
	// --------------------------------------

	FORCEINLINE FRotator GetDesiredCustomRotation() const
	{
		return DesiredCustomRotation;
	}

	inline void SetDesiredCustomRotation(const FRotator& NewRotation);

	inline void SetDesiredCustomRotation_LocalOnly(const FRotator& NewRotation);

	inline void SetDesiredCustomRotationYaw(float RotationYaw);

	inline void SetDesiredCustomRotationYaw_LocalOnly(float RotationYaw);

protected:
	/**
	 * Desired rotation of pawn owner. The character owner will always try to rotate smoothly to this DesiredCustomRotationYaw unless the 
	 * rotation behavior is overriden by setting 'bOrientRotationToMovement' or 'bUseControllerDesiredRotation' to true.
	 */
	UPROPERTY(Replicated)
	FRotator DesiredCustomRotation;

private:

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDesiredCustomRotation(const FRotator& NewRotation);
	virtual void Server_SetDesiredCustomRotation_Implementation(const FRotator& NewRotation);
	virtual bool Server_SetDesiredCustomRotation_Validate(const FRotator& NewRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDesiredCustomRotationYaw(float RotationYaw);
	virtual void Server_SetDesiredCustomRotationYaw_Implementation(float RotationYaw);
	virtual bool Server_SetDesiredCustomRotationYaw_Validate(float RotationYaw);

};

inline void UEODCharacterMovementComponent::SetDesiredCustomRotation(const FRotator& NewRotation)
{
	const float AngleTolerance = 1e-3f;
	if (!DesiredCustomRotation.Equals(NewRotation, AngleTolerance))
	{
		DesiredCustomRotation = NewRotation;
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetDesiredCustomRotation(NewRotation);
		}
	}
}

inline void UEODCharacterMovementComponent::SetDesiredCustomRotation_LocalOnly(const FRotator& NewRotation)
{
	DesiredCustomRotation = NewRotation;
}

inline void UEODCharacterMovementComponent::SetDesiredCustomRotationYaw(float RotationYaw)
{
	const float AngleTolerance = 1e-3f;
	if (!FMath::IsNearlyEqual(DesiredCustomRotation.Yaw, RotationYaw, AngleTolerance))
	{
		DesiredCustomRotation = FRotator(DesiredCustomRotation.Pitch, RotationYaw, DesiredCustomRotation.Roll);
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetDesiredCustomRotationYaw(RotationYaw);
		}
	}
}

inline void UEODCharacterMovementComponent::SetDesiredCustomRotationYaw_LocalOnly(float RotationYaw)
{
	SetDesiredCustomRotation_LocalOnly(FRotator(DesiredCustomRotation.Pitch, RotationYaw, DesiredCustomRotation.Roll));
}
