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
	UEODCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Perform rotation over deltaTime */
	virtual void PhysicsRotation(float DeltaTime) override;

	//~ @note: Overriding the default definition to fix a bug where Server drops/skips animation notifies.
	virtual void ServerMoveDual_Implementation(float TimeStamp0, FVector_NetQuantize10 InAccel0, uint8 PendingFlags, uint32 View0, float TimeStamp, FVector_NetQuantize10 InAccel, FVector_NetQuantize100 ClientLoc, uint8 NewFlags, uint8 ClientRoll, uint32 View, UPrimitiveComponent* ClientMovementBase, FName ClientBaseBoneName, uint8 ClientMovementMode) override;


	////////////////////////////////////////////////////////////////////////////////
	// Rotation
public:
	/** Returns the desired rotation yaw of pawn owner */
	FORCEINLINE float GetDesiredCustomRotationYaw() const
	{
		return DesiredCustomRotationYaw;
	}

	inline void SetDesiredCustomRotationYaw(float NewRotationYaw);

	/** Returns true if the pawn owner can rotate at all */
	FORCEINLINE bool CanRotate() const
	{
		return bCanRotate;
	}
	
	inline void SetCanRotate(bool bValue);

	void DoInstantRotation(float InstantRotationYaw);

private:
	/** Determines whether the pawn owner can currently rotate at all */
	UPROPERTY(Replicated)
	uint32 bCanRotate : 1;

	/**
	 * Desired rotation yaw of pawn owner. The character owner will always try to rotate smoothly to
	 * this DesiredCustomRotationYaw unless the rotation behavior is overriden by setting 'bOrientRotationToMovement'
	 * or 'bUseControllerDesiredRotation' to true.
	 */
	UPROPERTY(Replicated)
	float DesiredCustomRotationYaw;


	////////////////////////////////////////////////////////////////////////////////
	// Network
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCanRotate(bool bValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDesiredCustomRotationYaw(float NewRotationYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DoInstantRotation(float InstantRotationYaw);

};

inline void UEODCharacterMovementComponent::SetDesiredCustomRotationYaw(float NewRotationYaw)
{
	if (DesiredCustomRotationYaw != NewRotationYaw)
	{
		DesiredCustomRotationYaw = NewRotationYaw;
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetDesiredCustomRotationYaw(NewRotationYaw);
		}
	}
}

inline void UEODCharacterMovementComponent::SetCanRotate(bool bValue)
{
	if (bCanRotate != bValue)
	{
		bCanRotate = bValue;
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetCanRotate(bValue);
		}
	}
}
