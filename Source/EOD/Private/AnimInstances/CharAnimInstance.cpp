// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharAnimInstance.h"
#include "EODCharacterBase.h"


UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	IdleWalkRun_AnimationsBlendTime = 0.2f;
}

void UCharAnimInstance::NativeInitializeAnimation()
{
	EODCharacterOwner = TryGetPawnOwner() ? Cast<AEODCharacterBase>(TryGetPawnOwner()) : nullptr;

	OnMontageBlendingOut.AddDynamic(this, &UCharAnimInstance::HandleMontageBlendingOut);
	OnMontageEnded.AddDynamic(this, &UCharAnimInstance::HandleMontageEnded);
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(EODCharacterOwner))
	{
		if (!IsValid((EODCharacterOwner = TryGetPawnOwner() ? Cast<AEODCharacterBase>(TryGetPawnOwner()) : nullptr)))
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
	return EODCharacterOwner ? EODCharacterOwner->GetVelocity().Size() : 0.f;
}

bool UCharAnimInstance::IsBlocking() const
{
	return EODCharacterOwner ? EODCharacterOwner->IsBlocking() : false;
}

ECharMovementDirection UCharAnimInstance::GetIWRCharMovementDir() const
{
	return EODCharacterOwner ? EODCharacterOwner->GetCharacterMovementDirection() : ECharMovementDirection::None;
}

void UCharAnimInstance::TransitionBetweenMontages(
	UAnimMontage* TransitionFromMontage,
	UAnimMontage* TransitionToMontage,
	const FName Section,
	bool bZeroBlendOut)
{
	if (Montage_IsPlaying(TransitionFromMontage))
	{
		int32 MovingMontageSectionIndex = TransitionToMontage->GetSectionIndex(Section);
		int32 StandStillMontageSectionIndex = TransitionFromMontage->GetSectionIndex(Section);
		if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
		{
			float SectionStartTime;
			float SectionEndtime;
			TransitionFromMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
			float CurrentPosition = Montage_GetPosition(TransitionFromMontage);
			float SectionOffset = CurrentPosition - SectionStartTime;
			if (bZeroBlendOut)
			{
				Montage_Stop(0.f, TransitionFromMontage);
			}
			else
			{
				Montage_Stop(TransitionFromMontage->BlendOut.GetBlendTime(), TransitionFromMontage);
			}

			TransitionToMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
			float MontageStartPosition = SectionStartTime + SectionOffset;

			Montage_Play(TransitionToMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
		}
	}
}

float UCharAnimInstance::GetSectionLength(UAnimMontage* AnimMontage, FName SectionName)
{
	if (!AnimMontage || SectionName == NAME_None)
	{
		return -1.f;
	}

	int32 SectionIndex = AnimMontage->GetSectionIndex(SectionName);
	if (SectionIndex != INDEX_NONE)
	{
		float StartTime;
		float EndTime;
		AnimMontage->GetSectionStartAndEndTime(SectionIndex, StartTime, EndTime);
		return EndTime - StartTime;
	}

	return -1.f;
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
