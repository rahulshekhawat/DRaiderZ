// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharAnimInstance.h"
#include "EOD/Characters/EODCharacterBase.h"


UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	IdleWalkRun_AnimationsBlendTime = 0.2f;
}

void UCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	EODCharacterOwner = TryGetPawnOwner() ? Cast<AEODCharacterBase>(TryGetPawnOwner()) : nullptr;

	FScriptDelegate OutDelegate;
	OutDelegate.BindUFunction(this, FName("HandleMontageBlendingOut"));
	OnMontageBlendingOut.AddUnique(OutDelegate);

	FScriptDelegate EndDelegate;
	EndDelegate.BindUFunction(this, FName("HandleMontageEnded"));
	OnMontageEnded.AddUnique(EndDelegate);
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!EODCharacterOwner)
	{
		// If EOD character owner is null, attempt to recast pawn owner to EOD character
		if (!(EODCharacterOwner = TryGetPawnOwner() ? Cast<AEODCharacterBase>(TryGetPawnOwner()) : nullptr))
		{
			return;
		}
	}

	MovementSpeed = EODCharacterOwner->GetVelocity().Size();
	CharacterMovementDirection = EODCharacterOwner->GetCharacterMovementDirection();
	bIsBlocking = EODCharacterOwner->IsBlocking();

	/*
	if (EODCharacterOwner->IsIdle())
	{
		MovementSpeed = 0.f;
		CharacterMovementDirection = ECharMovementDirection::None;
	}
	else if (EODCharacterOwner->IsMoving())
	{
		MovementSpeed = EODCharacterOwner->GetVelocity().Size();
		CharacterMovementDirection = EODCharacterOwner->GetCharacterMovementDirection();
	}
	else if (EODCharacterOwner->IsBlocking())
	{
		bIsBlocking = true;
		MovementSpeed = EODCharacterOwner->GetVelocity().Size();
		CharacterMovementDirection = EODCharacterOwner->GetCharacterMovementDirection();
	}
	*/

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
	if (EODCharacterOwner)
	{
		return EODCharacterOwner->GetVelocity().Size();
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
	if (EODCharacterOwner)
	{
		return EODCharacterOwner->IsBlocking();
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

void UCharAnimInstance::HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(EODCharacterOwner))
	{
		EODCharacterOwner->OnMontageBlendingOut(AnimMontage, bInterrupted);
	}
}

void UCharAnimInstance::HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(EODCharacterOwner))
	{
		EODCharacterOwner->OnMontageEnded(AnimMontage, bInterrupted);
	}
}
