// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerAnimInstance.h"
#include "CharacterLibrary.h"
#include "PlayerCharacter.h"

#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	EODPlayerOwner = TryGetPawnOwner() ? Cast<APlayerCharacter>(TryGetPawnOwner()) : nullptr;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPlayerAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UPlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UPlayerAnimInstance::DoSeamlessTransitionBetweenStillOrMovingMontage(UAnimMontage* StandStillMontage, UAnimMontage* MovingMontage, const float ForwardAxisInput, const float RightAxisInput, const FName Section, bool bZeroBlendOut)
{
	if (ForwardAxisInput == 0.f && RightAxisInput == 0.f)
	{
		if (Montage_IsPlaying(MovingMontage))
		{
			int32 MovingMontageSectionIndex = MovingMontage->GetSectionIndex(Section);
			int32 StandStillMontageSectionIndex = StandStillMontage->GetSectionIndex(Section);
			if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
			{
				float SectionStartTime;
				float SectionEndtime;
				MovingMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
				float CurrentPosition = Montage_GetPosition(MovingMontage);
				float SectionOffset = CurrentPosition - SectionStartTime;
				if (bZeroBlendOut)
				{
					Montage_Stop(0.f, MovingMontage);
				}
				else
				{
					Montage_Stop(MovingMontage->BlendOut.GetBlendTime(), MovingMontage);
				}

				StandStillMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
				float MontageStartPosition = SectionStartTime + SectionOffset;

				Montage_Play(StandStillMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
			}
		}
	}
	else
	{
		if (Montage_IsPlaying(StandStillMontage))
		{
			int32 MovingMontageSectionIndex = MovingMontage->GetSectionIndex(Section);
			int32 StandStillMontageSectionIndex = StandStillMontage->GetSectionIndex(Section);
			if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
			{
				float SectionStartTime;
				float SectionEndtime;
				StandStillMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
				float CurrentPosition = Montage_GetPosition(StandStillMontage);
				float SectionOffset = CurrentPosition - SectionStartTime;
				if (bZeroBlendOut)
				{
					Montage_Stop(0.f, StandStillMontage);
				}
				else
				{
					Montage_Stop(StandStillMontage->BlendOut.GetBlendTime(), StandStillMontage);
				}

				MovingMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
				float MontageStartPosition = SectionStartTime + SectionOffset;

				Montage_Play(MovingMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
			}
		}
	}
}

void UPlayerAnimInstance::DoSeamlessTransitionBetweenStillOrMovingDirectionalMontage(
	UAnimMontage* StandStillMontage,
	UAnimMontage* MovingMontage,
	const float ForwardAxisInput,
	const float RightAxisInput,
	const FName BaseSection,
	bool bZeroBlendOut)
{
	if (ForwardAxisInput == 0.f && RightAxisInput == 0.f)
	{
		if (Montage_IsPlaying(MovingMontage))
		{
			FName CurrentMovingSection = Montage_GetCurrentSection(MovingMontage);
			int32 MovingMontageSectionIndex = MovingMontage->GetSectionIndex(CurrentMovingSection);
			int32 StandStillMontageSectionIndex = StandStillMontage->GetSectionIndex(BaseSection);
			if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
			{
				float SectionStartTime;
				float SectionEndtime;
				MovingMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
				float CurrentPosition = Montage_GetPosition(MovingMontage);
				float SectionOffset = CurrentPosition - SectionStartTime;
				if (bZeroBlendOut)
				{
					Montage_Stop(0.f, MovingMontage);
				}
				else
				{
					Montage_Stop(MovingMontage->BlendOut.GetBlendTime(), MovingMontage);
				}

				StandStillMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
				float MontageStartPosition = SectionStartTime + SectionOffset;

				Montage_Play(StandStillMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
			}
		}
	}
	else
	{
		FString SectionPostfix;
		if (ForwardAxisInput > 0)
		{
			SectionPostfix = "_F";
			// CharMoveDir = ECharMovementDirection::F;
		}
		else if (ForwardAxisInput < 0)
		{
			SectionPostfix = "_B";
			// CharMoveDir = ECharMovementDirection::B;
		}
		else if (RightAxisInput > 0)
		{
			SectionPostfix = "_R";
			// CharMoveDir = ECharMovementDirection::R;
		}
		else if (RightAxisInput < 0)
		{
			SectionPostfix = "_L";
			// CharMoveDir = ECharMovementDirection::L;
		}

		FName ExpectedSectionName = FName(*(BaseSection.ToString() + SectionPostfix));

		if (Montage_IsPlaying(StandStillMontage))
		{
			int32 MovingMontageSectionIndex = MovingMontage->GetSectionIndex(ExpectedSectionName);
			int32 StandStillMontageSectionIndex = StandStillMontage->GetSectionIndex(BaseSection);
			if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
			{
				float SectionStartTime;
				float SectionEndtime;
				StandStillMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
				float CurrentPosition = Montage_GetPosition(StandStillMontage);
				float SectionOffset = CurrentPosition - SectionStartTime;
				if (bZeroBlendOut)
				{
					Montage_Stop(0.2f, StandStillMontage);
				}
				else
				{
					Montage_Stop(StandStillMontage->BlendOut.GetBlendTime(), StandStillMontage);
				}

				MovingMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
				float MontageStartPosition = SectionStartTime + SectionOffset;

				Montage_Play(MovingMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
			}
		}
		else
		{
			FName CurrentMovingSection = Montage_GetCurrentSection(MovingMontage);
			
			if (CurrentMovingSection == ExpectedSectionName)
			{
				// do nothing
			}
			else
			{
				int32 CurrentSectionIndex = MovingMontage->GetSectionIndex(CurrentMovingSection);
				int32 ExpectedSectionIndex = MovingMontage->GetSectionIndex(ExpectedSectionName);
				if (CurrentSectionIndex != INDEX_NONE && ExpectedSectionIndex != INDEX_NONE)
				{
					float SectionStartTime;
					float SectionEndtime;
					MovingMontage->GetSectionStartAndEndTime(CurrentSectionIndex, SectionStartTime, SectionEndtime);
					float CurrentPosition = Montage_GetPosition(MovingMontage);
					float SectionOffset = CurrentPosition - SectionStartTime;
					if (bZeroBlendOut)
					{
						Montage_Stop(0.f, MovingMontage);
					}
					else
					{
						Montage_Stop(MovingMontage->BlendOut.GetBlendTime(), MovingMontage);
					}

					MovingMontage->GetSectionStartAndEndTime(ExpectedSectionIndex, SectionStartTime, SectionEndtime);
					float MontageStartPosition = SectionStartTime + SectionOffset;
					Montage_Play(MovingMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
				}
			}
		}
	}
}
