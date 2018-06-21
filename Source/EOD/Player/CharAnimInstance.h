// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "Animation/AnimInstance.h"
#include "CharAnimInstance.generated.h"

/**
 * 
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
	
protected:

	virtual void UpdateIdleAnimation();

	// @todo change moving to movement
	virtual void UpdateMovementAnimation();

	virtual void UpdateBlockAnimation();
	
	virtual void UpdateDodgeAnimation();

	UFUNCTION(BlueprintCallable, Category=BlockAnimationUpdate, meta=(BlueprintThreadSafe))
	virtual bool IsBlocking() const;

private:

	class ABaseCharacter* OwningCharacter;

	class ABaseCharacter* CastOwnerToBaseCharacter() const;
	
	
	
};
