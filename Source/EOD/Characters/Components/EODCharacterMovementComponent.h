// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

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

	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
private:
	/** 
	 * If this is set to true, character will smoothly rotate to 'DesiredRotationYaw'
	 * Once the rotation is complete, bUseCustomRotation is automatically set to false.
	 * This overrides the behavior of 'bOrientRotationToMovement' and 'bUseControllerDesiredRotation' while true.
	 */
	UPROPERTY(Replicated)
	bool bUseCustomRotation;

	UPROPERTY(Replicated)
	float DesiredCustomRotationYaw;

public:
	FORCEINLINE float GetDesiredCustomRotationYaw() const { return DesiredCustomRotationYaw; }

	FORCEINLINE bool IsUsingCustomRotation() const { return bUseCustomRotation; }

	FORCEINLINE void SetUseCustomRotation(bool bNewValue)
	{
		bUseCustomRotation = bNewValue;
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetUseCustomRotation(bNewValue);
		}
	}

	FORCEINLINE void SetDesiredCustomRotationYaw(float NewRotationYaw)
	{
		DesiredCustomRotationYaw = NewRotationYaw;
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetDesiredCustomRotationYaw(NewRotationYaw);
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// NETWORK
	////////////////////////////////////////////////////////////////////////////////
private:
	// UFUNCTION()
	// void OnRep_UseCustomRotation(bool bOldValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUseCustomRotation(bool bValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDesiredCustomRotationYaw(float NewRotationYaw);


};
