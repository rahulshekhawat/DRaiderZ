// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "Animation/AnimInstance.h"
#include "CharAnimInstance.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = LocomotionUpdate)
	float IdleWalkRun_AnimationsBlendTime;

	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	virtual bool IsBlocking() const;

	UFUNCTION(BlueprintCallable, Category = BlockAnimationUpdate, meta = (BlueprintThreadSafe))
	ECharMovementDirection GetIWRCharMovementDir() const;

	// virtual void UpdateIdleAnimation();

	// @todo change moving to movement
	// virtual void UpdateMovementAnimation();

	// virtual void UpdateBlockAnimation();
	
	// virtual void UpdateDodgeAnimation();


private:

	UFUNCTION()
	void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	class AEODCharacterBase* OwningCharacter;

	class AEODCharacterBase* CastOwnerToBaseCharacter() const;
	
	
	
};
