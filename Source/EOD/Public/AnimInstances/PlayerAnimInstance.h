// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponLibrary.h"
#include "CharacterLibrary.h"

#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;

/**
 * PlayerAnimInstance is the base class for animation blueprints of playable characters
 */
UCLASS()
class EOD_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Sets default values for blend times */
	UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	/** Binds delegates for OnMontageBlendingOut and OnMontageEnded delegates */
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;

	// --------------------------------------
	//  Animation Blueprint Update Variables
	// --------------------------------------
	
	/** Blend time for transitions between the animations coming from various state machines in main anim graph */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Player Anim Instance")
	float MasterStateMachine_AnimationsBlendTime;
	
	/** Blend time for transition between IdleWalkRun animations (of all state machines) */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Player Anim Instance")
	float IdleWalkRun_AnimationsBlendTime;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	bool bIsBlocking;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	bool bIsRunning;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	ECharMovementDirection CharacterMovementDirection;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	float MovementSpeed;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	float BlockMovementDirectionYaw;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	bool bPCTryingToMove;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Player Anim Instance")
	EWeaponType CurrentWeaponType;

	// --------------------------------------
	//  
	// --------------------------------------

	FORCEINLINE void OnTransitionableMontageTriggered(bool bIsCurrentMontageMovable)
	{
		bMovableMontagePlaying = bIsCurrentMontageMovable;
	}

private:
	UFUNCTION()
	void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	UPROPERTY()
	APlayerCharacter* EODPlayerOwner;

	/**
	 * A boolean used for seamless animation transitions.
	 * Determines whether the current montage playing is still-type or moving-type.
	 */
	UPROPERTY()
	bool bMovableMontagePlaying;

	FORCEINLINE void TransitionToMovableMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const FName Section,
		bool bZeroBlendOut = false)
	{
		TransitionBetweenMontages(StandStillMontage, MovingMontage, Section, bZeroBlendOut);
		bMovableMontagePlaying = true;
	}

	FORCEINLINE void TransitionToStillMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const FName Section,
		bool bZeroBlendOut = false)
	{
		TransitionBetweenMontages(MovingMontage, StandStillMontage, Section, bZeroBlendOut);
		bMovableMontagePlaying = false;
	}

	FORCEINLINE void DoSeamlessTransitionBetweenStillOrMovingMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const FName Section,
		bool bZeroBlendOut = false)
	{
		if (bPCTryingToMove)
		{
			TransitionBetweenMontages(StandStillMontage, MovingMontage, Section, bZeroBlendOut);
			bMovableMontagePlaying = true;
		}
		else
		{
			TransitionBetweenMontages(MovingMontage, StandStillMontage, Section, bZeroBlendOut);
			bMovableMontagePlaying = false;
		}
	}

	void TransitionBetweenMontages(
		UAnimMontage* TransitionFromMontage,
		UAnimMontage* TransitionToMontage,
		const FName Section,
		bool bZeroBlendOut = false);

	// DEPRECATED - doesn't work in multiplayer
	void DoSeamlessTransitionBetweenStillOrMovingMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const float ForwardAxisInput,
		const float RightAxisInput,
		const FName Section,
		bool bZeroBlendOut = false);

	void DoSeamlessTransitionBetweenStillOrMovingDirectionalMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const float ForwardAxisInput,
		const float RightAxisInput,
		const FName BaseSection,
		bool bZeroBlendOut = false);


	bool bJumpAnimationPlayingLocally;
	bool bJumpLandAnimationPlayingLocally;

};
