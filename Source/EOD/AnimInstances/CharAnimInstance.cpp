// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/AnimInstances/CharAnimInstance.h"
#include "EOD/Characters/EODCharacterBase.h"


UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	IdleWalkRun_AnimationsBlendTime = 0.2f;
}

void UCharAnimInstance::NativeInitializeAnimation()
{
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
}

void UCharAnimInstance::NativePostEvaluateAnimation()
{
}

void UCharAnimInstance::NativeUninitializeAnimation()
{
}

float UCharAnimInstance::GetMovementSpeed() const
{
	if (IsValid(EODCharacterOwner))
	{
		return EODCharacterOwner->GetVelocity().Size();
	}

	return 0.0f;
}

bool UCharAnimInstance::IsBlocking() const
{
	if (IsValid(EODCharacterOwner))
	{
		return EODCharacterOwner->IsBlocking();
	}

	return false;
}

ECharMovementDirection UCharAnimInstance::GetIWRCharMovementDir() const
{
	if (IsValid(EODCharacterOwner))
	{
		return EODCharacterOwner->GetCharacterMovementDirection();
	}

	return ECharMovementDirection::None;
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
