// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharAnimInstance.h"
#include "EODCharacterBase.h"


UCharAnimInstance::UCharAnimInstance(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	// IdleWalkRun_AnimationsBlendTime = 0.2f;
}

void UCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = CastOwnerToBaseCharacter();
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter)
	{
		OwningCharacter = CastOwnerToBaseCharacter();
		// Return because we are not sure if OwningCharacter is nullptr right now or not
		return;
	}

	if (OwningCharacter->IsIdle())
	{
		UpdateIdleAnimation();
	}
	else if (OwningCharacter->IsMoving())
	{
		UpdateMovementAnimation();
	}
	else if (OwningCharacter->IsBlocking())
	{
		UpdateBlockAnimation();
	}
	else if (OwningCharacter->IsDodging())
	{
		UpdateDodgeAnimation();
	}
}

void UCharAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UCharAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UCharAnimInstance::UpdateIdleAnimation()
{
	// IdleWalkRun_MovementSpeed = 0.f;
}

void UCharAnimInstance::UpdateMovementAnimation()
{
	// IdleWalkRun_MovementSpeed = OwningCharacter->GetVelocity().Size();
}

void UCharAnimInstance::UpdateBlockAnimation()
{
}

void UCharAnimInstance::UpdateDodgeAnimation()
{
}

bool UCharAnimInstance::IsBlocking() const
{
	return true;
}

AEODCharacterBase * UCharAnimInstance::CastOwnerToBaseCharacter() const
{
	AEODCharacterBase* BaseCharacter = nullptr;

	if (TryGetPawnOwner())
	{
		BaseCharacter = Cast<AEODCharacterBase>(TryGetPawnOwner());
	}

	return BaseCharacter;
}
