// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Characters/PlayerCharacter.h"
#include "EOD/Statics/WeaponLibrary.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = MasterStateUpdate)
	float MasterStateMachine_AnimationsBlendTime;
	
	/** Blend time for transition between IdleWalkRun animations (of all state machines) */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = LocomotionUpdate)
	float IdleWalkRun_AnimationsBlendTime;


	//~ Begin functions to update animation blueprint
	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	bool IsBlocking() const;

	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	bool IsFastRunning() const;

	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	ECharMovementDirection GetIWRCharMovementDir() const;
	
	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	float GetMovementSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	float GetBlockMovementDirectionYaw() const;

	UFUNCTION(BlueprintCallable, Category = LocomotionUpdate, meta = (BlueprintThreadSafe))
	bool IsPCTryingToMove() const;
	
	// UFUNCTION(BlueprintCallable, Category = MasterStateUpdate, meta = (BlueprintThreadSafe))
	// EWeaponAnimationType GetWeaponAnimationType() const;

	UFUNCTION(BlueprintCallable, Category = MasterStateUpdate, meta = (BlueprintThreadSafe))
	EWeaponType GetWeaponAnimationType() const;
	//~ End functions to update animation blueprint

private:
	
	UFUNCTION()
	void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	UPROPERTY()
	APlayerCharacter* OwningPlayer;

	FORCEINLINE APlayerCharacter* GetPlayerOwner();

	FORCEINLINE void SetupOwningPlayer();

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

FORCEINLINE APlayerCharacter* UPlayerAnimInstance::GetPlayerOwner()
{
	if (OwningPlayer)
	{
		return OwningPlayer;
	}
	else
	{
		USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
		if (AActor* OwnerActor = OwnerComponent->GetOwner())
		{
			return Cast<APlayerCharacter>(OwnerComponent->GetOwner());
		}
	}

	return nullptr;
}

FORCEINLINE void UPlayerAnimInstance::SetupOwningPlayer()
{
	OwningPlayer = GetPlayerOwner();
}
