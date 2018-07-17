// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/WeaponLibrary.h"
#include "Statics/CharacterLibrary.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = IdleWalkRunUpdate)
	float MasterStateMachine_AnimationsBlendTime;
	
	/** Blend time for transition between IdleWalkRun animations (of all state machines) */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = IdleWalkRunUpdate)
	float IdleWalkRun_AnimationsBlendTime;


	//~ Begin functions to update animation blueprint
	UFUNCTION(BlueprintCallable, Category = BlockAnimationUpdate, meta = (BlueprintThreadSafe))
	bool IsBlocking() const;
	
	UFUNCTION(BlueprintCallable, Category = BlockAnimationUpdate, meta = (BlueprintThreadSafe))
	ECharMovementDirection GetIWRCharMovementDir() const;
	
	UFUNCTION(BlueprintCallable, Category = BlockAnimationUpdate, meta = (BlueprintThreadSafe))
	float GetMovementSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = BlockAnimationUpdate, meta = (BlueprintThreadSafe))
	float GetBlockMovementDirectionYaw() const;
	
	UFUNCTION(BlueprintCallable, Category = BlockAnimationUpdate, meta = (BlueprintThreadSafe))
	EWeaponAnimationType GetWeaponAnimationType() const;
	//~ End functions to update animation blueprint

private:
	
	UFUNCTION()
	void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	class APlayerCharacter* OwningPlayer;

	class APlayerCharacter* CastPawnOwnerToPlayerCharacter();
	
	
};
