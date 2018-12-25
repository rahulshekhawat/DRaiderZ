// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/WeaponLibrary.h"
#include "EOD/Statics/CharacterLibrary.h"

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
	/** Sets default values for blend times */
	UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	/** Binds delegates for OnMontageBlendingOut and OnMontageEnded delegates */
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;
	
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

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	bool IsBlocking() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	bool IsFastRunning() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	ECharMovementDirection GetIWRCharMovementDir() const;
	
	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	float GetMovementSpeed() const;
	
	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	float GetBlockMovementDirectionYaw() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	bool IsPCTryingToMove() const;
	
	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "Player Anim Instance", meta = (BlueprintThreadSafe, DeprecatedFunction))
	EWeaponType GetWeaponAnimationType() const;

private:
	UFUNCTION()
	void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	UPROPERTY()
	APlayerCharacter* EODPlayerOwner;

	void DoSeamlessTransitionBetweenStillOrMovingMontage(UAnimMontage* StandStillMontage,
														 UAnimMontage* MovingMontage,
														 const float ForwardAxisInput,
														 const float RightAxisInput,
														 const FName Section,
														 bool bZeroBlendOut = false);

	void DoSeamlessTransitionBetweenStillOrMovingDirectionalMontage(UAnimMontage* StandStillMontage,
																	UAnimMontage* MovingMontage,
																	const float ForwardAxisInput,
																	const float RightAxisInput,
																	const FName BaseSection,
																	bool bZeroBlendOut = false);

};
