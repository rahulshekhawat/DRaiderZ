// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponLibrary.h"
#include "CharacterLibrary.h"

#include "HumanCharAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;

/**
 * PlayerAnimInstance is the base class for animation blueprints of playable characters
 */
UCLASS()
class EOD_API UPlayerAnimInstance : public UHumanCharAnimInstance
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

private:

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

	UPROPERTY(Transient)
	APlayerCharacter* EODPlayerOwner;

};
