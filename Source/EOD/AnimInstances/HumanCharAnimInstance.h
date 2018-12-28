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
	UHumanCharAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativePostEvaluateAnimation() override;

	virtual void NativeUninitializeAnimation() override;

	/** Blend time for transitions between the animations coming from various state machines in main anim graph */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Human Character Animation")
	float MasterStateMachine_AnimationsBlendTime;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Human Character Animation")
	EWeaponType CurrentWeaponType;

private:
	UPROPERTY()
	AHumanCharacter* HumanOwner;


};
