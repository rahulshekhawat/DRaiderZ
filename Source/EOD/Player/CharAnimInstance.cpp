// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharAnimInstance.h"
#include "EODCharacterBase.h"


UCharAnimInstance::UCharAnimInstance(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	IdleWalkRun_AnimationsBlendTime = 0.2f;
}

void UCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwningCharacter = CastOwnerToBaseCharacter();

	FScriptDelegate OutDelegate;
	OutDelegate.BindUFunction(this, FName("HandleMontageBlendingOut"));
	OnMontageBlendingOut.AddUnique(OutDelegate);

	FScriptDelegate EndDelegate;
	EndDelegate.BindUFunction(this, FName("HandleMontageEnded"));
	OnMontageEnded.AddUnique(EndDelegate);
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

	/*
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
	*/
}

void UCharAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UCharAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

float UCharAnimInstance::GetMovementSpeed() const
{
	if (OwningCharacter)
	{
		return OwningCharacter->GetVelocity().Size();
	}

	return 0.0f;
}

/*
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
*/

bool UCharAnimInstance::IsBlocking() const
{
	if (OwningCharacter)
	{
		return OwningCharacter->IsBlocking();
	}

	return false;
}

ECharMovementDirection UCharAnimInstance::GetIWRCharMovementDir() const
{
	if (GetMovementSpeed() == 0)
	{
		return ECharMovementDirection::None;
	}
	else
	{
		return ECharMovementDirection::F;
	}
}

void UCharAnimInstance::HandleMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
	OwningCharacter->OnMontageBlendingOut(AnimMontage, bInterrupted);
}

void UCharAnimInstance::HandleMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
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
