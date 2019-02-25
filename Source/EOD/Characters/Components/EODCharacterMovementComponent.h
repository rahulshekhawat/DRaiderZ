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


	////////////////////////////////////////////////////////////////////////////////
	// Rotation
public:
	FORCEINLINE FRotator GetDesiredCustomRotation() const
	{
		return DesiredCustomRotation;
	}

	inline void SetDesiredCustomRotation(const FRotator& NewRotation);

protected:
	/**
	 * Desired rotation of pawn owner. The character owner will always try to rotate smoothly to this DesiredCustomRotationYaw unless the 
	 * rotation behavior is overriden by setting 'bOrientRotationToMovement' or 'bUseControllerDesiredRotation' to true.
	 */
	UPROPERTY(Replicated)
	FRotator DesiredCustomRotation;


	////////////////////////////////////////////////////////////////////////////////
	// Network
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDesiredCustomRotation(const FRotator& NewRotation);

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
