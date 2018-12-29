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

	virtual void BeginPlay() override;

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Perform rotation over deltaTime */
	virtual void PhysicsRotation(float DeltaTime) override;

	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
private:
	/**
	 * Desired rotation yaw of CharacterOwner. The character owner will always try to rotate smoothly to
	 * this DesiredCustomRotationYaw unless the rotation behavior is overriden by setting 'bOrientRotationToMovement'
	 * or 'bUseControllerDesiredRotation' to true.
	 */
	UPROPERTY(Replicated)
	float DesiredCustomRotationYaw;

public:
	FORCEINLINE float GetDesiredCustomRotationYaw() const { return DesiredCustomRotationYaw; }

	FORCEINLINE void SetDesiredCustomRotationYaw(float NewRotationYaw)
	{
		DesiredCustomRotationYaw = NewRotationYaw;
		if (CharacterOwner && CharacterOwner->Role < ROLE_Authority)
		{
			Server_SetDesiredCustomRotationYaw(NewRotationYaw);
		}
	}

	void DoInstantRotation(float InstantRotationYaw);

	////////////////////////////////////////////////////////////////////////////////
	// NETWORK
	////////////////////////////////////////////////////////////////////////////////
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDesiredCustomRotationYaw(float NewRotationYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DoInstantRotation(float InstantRotationYaw);

};
