// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "Animation/AnimInstance.h"
#include "CharAnimInstance.generated.h"

class AEODCharacterBase;

/**
 * CharAnimInstance is the base class for animation blueprints of non-playable characters
 */
UCLASS()
class EOD_API UCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UCharAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;

	// --------------------------------------
	//  Animation Blueprint Update Variables
	// --------------------------------------

	/** Blend time for transition between IdleWalkRun animations (of all state machines) */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Animation Update")
	float IdleWalkRun_AnimationsBlendTime;
	
	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	float BlockMovementDirectionYaw;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	float MovementSpeed;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	bool bIsBlocking;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	bool bIsRunning;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	bool bPCTryingToMove;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	ECharMovementDirection CharacterMovementDirection;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Animation Update", meta = (BlueprintThreadSafe, DeprecatedFunction))
	float GetMovementSpeed() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Animation Update", meta = (BlueprintThreadSafe, DeprecatedFunction))
	virtual bool IsBlocking() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Animation Update", meta = (BlueprintThreadSafe, DeprecatedFunction))
	ECharMovementDirection GetIWRCharMovementDir() const;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	inline void DoSeamlessTransitionBetweenStillOrMovingMontage(
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

	inline void TransitionToMovableMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const FName Section,
		bool bZeroBlendOut = false)
	{
		TransitionBetweenMontages(StandStillMontage, MovingMontage, Section, bZeroBlendOut);
		bMovableMontagePlaying = true;
	}

	inline void TransitionToStillMontage(
		UAnimMontage* StandStillMontage,
		UAnimMontage* MovingMontage,
		const FName Section,
		bool bZeroBlendOut = false)
	{
		TransitionBetweenMontages(MovingMontage, StandStillMontage, Section, bZeroBlendOut);
		bMovableMontagePlaying = false;
	}

	void TransitionBetweenMontages(
		UAnimMontage* TransitionFromMontage,
		UAnimMontage* TransitionToMontage,
		const FName Section,
		bool bZeroBlendOut = false);

	/** Returns the length of a section in an anim montage. Returns -1.f if the section isn't found. */
	float GetSectionLength(UAnimMontage* AnimMontage, FName SectionName);
	
	// --------------------------------------
	//  Events
	// --------------------------------------

	FORCEINLINE void OnTransitionableMontageTriggered(bool bIsCurrentMontageMovable) { bMovableMontagePlaying = bIsCurrentMontageMovable; }

protected:

	UFUNCTION()
	virtual void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	virtual void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	UPROPERTY(Transient)
	AEODCharacterBase* EODCharacterOwner;
	
	/**
	 * A boolean used for seamless animation transitions.
	 * Determines whether the current montage playing is still-type or moving-type.
	 */
	UPROPERTY(Transient)
	bool bMovableMontagePlaying;
	
};
