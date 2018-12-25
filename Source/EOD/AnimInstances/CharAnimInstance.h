// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"
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
	UCharAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;

	/** Blend time for transition between IdleWalkRun animations (of all state machines) */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "EOD Character Animation")
	float IdleWalkRun_AnimationsBlendTime;
	
	UPROPERTY(Transient, BlueprintReadonly, Category = "EOD Character Animation")
	float BlockMovementDirectionYaw;

	UPROPERTY(Transient, BlueprintReadonly, Category = "EOD Character Animation")
	float MovementSpeed;

	UPROPERTY(Transient, BlueprintReadonly, Category = "EOD Character Animation")
	bool bIsBlocking;

	UPROPERTY(Transient, BlueprintReadonly, Category = "EOD Character Animation")
	bool bIsRunning;

	UPROPERTY(Transient, BlueprintReadonly, Category = "EOD Character Animation")
	bool bPCTryingToMove;

	UPROPERTY(Transient, BlueprintReadonly, Category = "EOD Character Animation")
	ECharMovementDirection CharacterMovementDirection;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "EOD Character Animation", meta = (BlueprintThreadSafe, DeprecatedFunction))
	float GetMovementSpeed() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "EOD Character Animation", meta = (BlueprintThreadSafe, DeprecatedFunction))
	virtual bool IsBlocking() const;

	// DEPRECATED
	UFUNCTION(BlueprintCallable, Category = "EOD Character Animation", meta = (BlueprintThreadSafe, DeprecatedFunction))
	ECharMovementDirection GetIWRCharMovementDir() const;

private:
	UFUNCTION()
	void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	UPROPERTY()
	AEODCharacterBase* EODCharacterOwner;
	
	
	
};
