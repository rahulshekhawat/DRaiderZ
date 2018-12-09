// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerAnimInstance.h"
#include "EOD/Player/PlayerCharacter.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	MasterStateMachine_AnimationsBlendTime = 0.f;
	IdleWalkRun_AnimationsBlendTime = 0.2f;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SetupOwningPlayer();

	FScriptDelegate OutDelegate;
	OutDelegate.BindUFunction(this, FName("HandleMontageBlendingOut"));
	OnMontageBlendingOut.AddUnique(OutDelegate);
	
	FScriptDelegate EndDelegate;
	EndDelegate.BindUFunction(this, FName("HandleMontageEnded"));
	OnMontageEnded.AddUnique(EndDelegate);

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningPlayer || !OwningPlayer->GetActiveAnimationReferences())
	{
#if MESSAGE_LOGGING_ENABLED
		UKismetSystemLibrary::PrintString(this, FString("Either character or animation references are null"));
#endif // MESSAGE_LOGGING_ENABLED
		return;
	}

	FPlayerAnimationReferencesTableRow* AnimationReferences = OwningPlayer->GetActiveAnimationReferences();

	if (OwningPlayer->IsDead())
	{
		UAnimMontage* DeathMontage = AnimationReferences->Die.Get();
		if (DeathMontage && !Montage_IsPlaying(DeathMontage))
		{
			Montage_Play(DeathMontage);
		}

		return;
	}
	else
	{
		UAnimMontage* DeathMontage = AnimationReferences->Die.Get();
		if (DeathMontage && Montage_IsPlaying(DeathMontage))
		{
			Montage_JumpToSection(FName("Rebirth"), DeathMontage);
			return;
		}
		else
		{
			// do nothing
		}
	}

	// @todo
	/*
	// When the character is falling and jumping
	if (OwningPlayer->GetMovementComponent()->IsFalling() && OwningPlayer->IsJumping())
	{

	}
	// When the character is not falling but jumping
	else if (!OwningPlayer->GetCharacterMovement()->IsFalling() && OwningPlayer->IsJumping())
	{

	}
	// When character is falling but not jumping
	else if (OwningPlayer->GetCharacterMovement()->IsFalling() && !OwningPlayer->IsJumping() && OwningPlayer->IsIdleOrMoving())
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage)
		{
			if (Montage_IsPlaying(JumpMontage))
			{
				OwningPlayer->SetCharacterState(ECharacterState::Jumping);
			}
			else
			{
				OwningPlayer->SetCharacterState(ECharacterState::Jumping);
				Montage_Play(JumpMontage);
				return;
			}
		}
	}
	*/


	if (OwningPlayer->GetMovementComponent()->IsFalling())
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage && !Montage_IsPlaying(JumpMontage))
		{
			Montage_Play(JumpMontage);
			if (OwningPlayer->GetCharacterState() != ECharacterState::Jumping)
			{
				OwningPlayer->SetCharacterState(ECharacterState::Jumping);
			}
			return;
		}

		return;
	}
	else
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage && Montage_IsPlaying(JumpMontage))
		{
			FName CurrentSection = Montage_GetCurrentSection(JumpMontage);
			if (CurrentSection != UCharacterLibrary::SectionName_JumpEnd)
			{
				Montage_JumpToSection(UCharacterLibrary::SectionName_JumpEnd, JumpMontage);
			}
			return;
		}
		else
		{
			// do nothing
		}
	}

	if (OwningPlayer->IsSwitchingWeapon() && OwningPlayer->GetEquippedWeaponAnimationReferences())
	{
		UAnimMontage* FullBodySwitchMontage = OwningPlayer->GetEquippedWeaponAnimationReferences()->WeaponSwitchFullBody.Get();
		UAnimMontage* UpperBodySwitchMontage = OwningPlayer->GetEquippedWeaponAnimationReferences()->WeaponSwitchUpperBody.Get();

		FName MontageSection;
		// If weapon is currently sheathed then it means we are playing sheathe animation
		if (OwningPlayer->IsWeaponSheathed())
		{
			MontageSection = UCharacterLibrary::SectionName_SheatheWeapon;
		}
		else
		{
			MontageSection = UCharacterLibrary::SectionName_UnsheatheWeapon;
		}

		if (FullBodySwitchMontage && UpperBodySwitchMontage)
		{
			float ForwardAxisValue = OwningPlayer->InputComponent->GetAxisValue(FName("MoveForward"));
			float RightAxisValue = OwningPlayer->InputComponent->GetAxisValue(FName("MoveRight"));

			DoSeamlessTransitionBetweenStillOrMovingMontage(FullBodySwitchMontage, UpperBodySwitchMontage, ForwardAxisValue, RightAxisValue, MontageSection, true);
		}
	}
	
	if (OwningPlayer->IsUsingAnySkill() && OwningPlayer->SkillAllowsMovement())
	{
		FSkillTableRow* ActiveSkill = OwningPlayer->GetCurrentActiveSkill();
		if (ActiveSkill)
		{
			UAnimMontage* FullBodySkillMontage = ActiveSkill->AnimMontage.Get();
			UAnimMontage* UpperBodySkillMontage = ActiveSkill->UpperBodyAnimMontage.Get();
			FName MontageSection = ActiveSkill->SkillStartMontageSectionName;

			if (FullBodySkillMontage && UpperBodySkillMontage)
			{
				float ForwardAxisValue = OwningPlayer->InputComponent->GetAxisValue(FName("MoveForward"));
				float RightAxisValue = OwningPlayer->InputComponent->GetAxisValue(FName("MoveRight"));

				if (OwningPlayer->SkillHasDirectionalAnimations())
				{
					DoSeamlessTransitionBetweenStillOrMovingDirectionalMontage(FullBodySkillMontage, UpperBodySkillMontage, ForwardAxisValue, RightAxisValue, MontageSection);
				}
				else
				{
					DoSeamlessTransitionBetweenStillOrMovingMontage(FullBodySkillMontage, UpperBodySkillMontage, ForwardAxisValue, RightAxisValue, MontageSection);
				}

			}
		}
	}
}

void UPlayerAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UPlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

bool UPlayerAnimInstance::IsBlocking() const
{
	if (OwningPlayer)
	{
		return OwningPlayer->IsBlocking();
	}

	return false;
}

bool UPlayerAnimInstance::IsFastRunning() const
{
	if (OwningPlayer)
	{
		return OwningPlayer->IsFastRunning();
	}

	return false;
}

ECharMovementDirection UPlayerAnimInstance::GetIWRCharMovementDir() const
{
	if (OwningPlayer)
	{
		return OwningPlayer->IWR_CharacterMovementDirection;
	}

	return ECharMovementDirection::None;
}

float UPlayerAnimInstance::GetMovementSpeed() const
{
	if (OwningPlayer)
	{
		return OwningPlayer->GetVelocity().Size();
	}

	return 0.f;
}

float UPlayerAnimInstance::GetBlockMovementDirectionYaw() const
{
	if (OwningPlayer)
	{
		return OwningPlayer->BlockMovementDirectionYaw;
	}

	return 0.f;
}

EWeaponType UPlayerAnimInstance::GetWeaponAnimationType() const
{
	if (OwningPlayer && !OwningPlayer->IsWeaponSheathed())
	{
		return OwningPlayer->GetEquippedWeaponType();
	}

	return EWeaponType::None;
}

void UPlayerAnimInstance::HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (OwningPlayer)
	{
		OwningPlayer->OnMontageBlendingOut(AnimMontage, bInterrupted);
	}

	/*
	if(!bInterrupted)
	{
		if (AnimMontage == OwningPlayer->GetActiveAnimationReferences()->AnimationMontage_Jump)
		{
			// OwningPlayer->GetCharacterState() == ECharacterState::IdleWalkRun;
		}
		else if (AnimMontage == OwningPlayer->GetActiveAnimationReferences()->AnimationMontage_Dodge)
		{
			// OwningPlayer->GetCharacterState() == ECharacterState::IdleWalkRun;
			// @todo
			// OwningPlayer-> UpdateMovementAnimation();
		}
		else
		{
			// OwningPlayer->GetCharacterState() == ECharacterState::IdleWalkRun;
		}
	}
	*/
}

void UPlayerAnimInstance::HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (OwningPlayer)
	{
		OwningPlayer->OnMontageEnded(AnimMontage, bInterrupted);
	}
}

/*
APlayerCharacter * UPlayerAnimInstance::CastPawnOwnerToPlayerCharacter()
{
	APlayerCharacter* PlayerChar = nullptr;

	if (TryGetPawnOwner())
	{
		PlayerChar = Cast<APlayerCharacter>(TryGetPawnOwner());
	}

	return PlayerChar;
}
*/

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

void UPlayerAnimInstance::DoSeamlessTransitionBetweenStillOrMovingDirectionalMontage(UAnimMontage* StandStillMontage, UAnimMontage* MovingMontage, const float ForwardAxisInput, const float RightAxisInput, const FName BaseSection, bool bZeroBlendOut)
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
