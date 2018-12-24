// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharAnimInstance.h"
#include "HumanCharAnimInstance.generated.h"

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

	
};
