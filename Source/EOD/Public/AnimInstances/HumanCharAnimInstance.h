// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharAnimInstance.h"
#include "HumanCharAnimInstance.generated.h"

class AHumanCharacter;

/**
 * 
 */
UCLASS()
class EOD_API UHumanCharAnimInstance : public UCharAnimInstance
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UHumanCharAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;

	// --------------------------------------
	//  Animation Blueprint Update Variables
	// --------------------------------------

	/** Blend time for transitions between the animations coming from various state machines in main anim graph */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Animation Update")
	float MasterStateMachine_AnimationsBlendTime;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Update")
	EWeaponType CurrentWeaponType;

protected:

	// --------------------------------------
	//  Events
	// --------------------------------------

	virtual void HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) override;
	virtual void HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) override;

	bool bJumpAnimationPlayingLocally;
	bool bJumpLandAnimationPlayingLocally;

private:

	UPROPERTY(Transient)
	AHumanCharacter* HumanOwner;

};
