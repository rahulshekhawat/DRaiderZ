// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/WeaponLibrary.h"
#include "Statics/CharacterLibrary.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = IdleWalkRunUpdate)
	float MasterStateMachine_AnimationsBlendTime;
	
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
