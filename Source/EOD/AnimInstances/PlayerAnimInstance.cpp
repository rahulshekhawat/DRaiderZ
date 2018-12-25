// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/AnimInstances/PlayerAnimInstance.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/Characters/PlayerCharacter.h"

#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	MasterStateMachine_AnimationsBlendTime = 0.f;
	IdleWalkRun_AnimationsBlendTime = 0.2f;
	CurrentWeaponType = EWeaponType::None;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	EODPlayerOwner = TryGetPawnOwner() ? Cast<APlayerCharacter>(TryGetPawnOwner()) : nullptr;

	FScriptDelegate OutDelegate;
	OutDelegate.BindUFunction(this, FName("HandleMontageBlendingOut"));
	OnMontageBlendingOut.AddUnique(OutDelegate);
	
	FScriptDelegate EndDelegate;
	EndDelegate.BindUFunction(this, FName("HandleMontageEnded"));
	OnMontageEnded.AddUnique(EndDelegate);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!EODPlayerOwner)
	{
		if (!(EODPlayerOwner = TryGetPawnOwner() ? Cast<APlayerCharacter>(TryGetPawnOwner()) : nullptr))
		{
			return;
		}
	}

	MovementSpeed				= EODPlayerOwner->GetVelocity().Size();
	CharacterMovementDirection	= EODPlayerOwner->GetCharacterMovementDirection();
	bIsBlocking					= EODPlayerOwner->IsBlocking();
	bIsRunning					= EODPlayerOwner->IsRunning();
	BlockMovementDirectionYaw	= EODPlayerOwner->BlockMovementDirectionYaw;
	bPCTryingToMove				= EODPlayerOwner->IsPCTryingToMove();

	CurrentWeaponType = EODPlayerOwner->IsWeaponSheathed() ? EWeaponType::None : EODPlayerOwner->GetEquippedWeaponType();

	FPlayerAnimationReferencesTableRow* AnimationReferences = EODPlayerOwner->GetActiveAnimationReferences();
	if (!AnimationReferences)
	{
		return;
	}


#if EOD_TEST_CODE_ENABLED
	if (EODPlayerOwner->IsSwitchingWeapon() && EODPlayerOwner->GetEquippedWeaponAnimationReferences())
	{
		UAnimMontage* FullBodySwitchMontage = EODPlayerOwner->GetEquippedWeaponAnimationReferences()->WeaponSwitchFullBody.Get();
		UAnimMontage* UpperBodySwitchMontage = EODPlayerOwner->GetEquippedWeaponAnimationReferences()->WeaponSwitchUpperBody.Get();

		FName MontageSection;
		// If weapon is currently sheathed then it means we are playing sheathe animation
		if (EODPlayerOwner->IsWeaponSheathed())
		{
			MontageSection = UCharacterLibrary::SectionName_SheatheWeapon;
		}
		else
		{
			MontageSection = UCharacterLibrary::SectionName_UnsheatheWeapon;
		}

		if (FullBodySwitchMontage && UpperBodySwitchMontage)
		{
			// float ForwardAxisValue = EODPlayerOwner->InputComponent->GetAxisValue(FName("MoveForward"));
			// float RightAxisValue = EODPlayerOwner->InputComponent->GetAxisValue(FName("MoveRight"));
			float ForwardAxisValue = EODPlayerOwner->ForwardAxisValue;
			float RightAxisValue = EODPlayerOwner->RightAxisValue;
			DoSeamlessTransitionBetweenStillOrMovingMontage(FullBodySwitchMontage, UpperBodySwitchMontage, ForwardAxisValue, RightAxisValue, MontageSection, true);
		}
	}

	return;
#endif

	if (EODPlayerOwner->IsDead())
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
	if (EODPlayerOwner->GetMovementComponent()->IsFalling() && EODPlayerOwner->IsJumping())
	{

	}
	// When the character is not falling but jumping
	else if (!EODPlayerOwner->GetCharacterMovement()->IsFalling() && EODPlayerOwner->IsJumping())
	{

	}
	// When character is falling but not jumping
	else if (EODPlayerOwner->GetCharacterMovement()->IsFalling() && !EODPlayerOwner->IsJumping() && EODPlayerOwner->IsIdleOrMoving())
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage)
		{
			if (Montage_IsPlaying(JumpMontage))
			{
				EODPlayerOwner->SetCharacterState(ECharacterState::Jumping);
			}
			else
			{
				EODPlayerOwner->SetCharacterState(ECharacterState::Jumping);
				Montage_Play(JumpMontage);
				return;
			}
		}
	}
	*/


	if (EODPlayerOwner->GetMovementComponent()->IsFalling())
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage && !Montage_IsPlaying(JumpMontage))
		{
			Montage_Play(JumpMontage);
			if (EODPlayerOwner->GetCharacterState() != ECharacterState::Jumping)
			{
				EODPlayerOwner->SetCharacterState(ECharacterState::Jumping);
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

	/*
	if (EODPlayerOwner->IsSwitchingWeapon())
	{
		if (EODPlayerOwner->IsPCTryingToMove())
		{
			RootMotionMode = ERootMotionMode::IgnoreRootMotion;
		}
		else
		{
			RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
		}
	
	}
	*/

	/*
	if (EODPlayerOwner->IsSwitchingWeapon() && EODPlayerOwner->GetEquippedWeaponAnimationReferences())
	{
		UAnimMontage* FullBodySwitchMontage = EODPlayerOwner->GetEquippedWeaponAnimationReferences()->WeaponSwitchFullBody.Get();
		UAnimMontage* UpperBodySwitchMontage = EODPlayerOwner->GetEquippedWeaponAnimationReferences()->WeaponSwitchUpperBody.Get();

		FName MontageSection;
		// If weapon is currently sheathed then it means we are playing sheathe animation
		if (EODPlayerOwner->IsWeaponSheathed())
		{
			MontageSection = UCharacterLibrary::SectionName_SheatheWeapon;
		}
		else
		{
			MontageSection = UCharacterLibrary::SectionName_UnsheatheWeapon;
		}

		if (FullBodySwitchMontage && UpperBodySwitchMontage)
		{
			float ForwardAxisValue = EODPlayerOwner->InputComponent->GetAxisValue(FName("MoveForward"));
			float RightAxisValue = EODPlayerOwner->InputComponent->GetAxisValue(FName("MoveRight"));

			DoSeamlessTransitionBetweenStillOrMovingMontage(FullBodySwitchMontage, UpperBodySwitchMontage, ForwardAxisValue, RightAxisValue, MontageSection, true);
		}
	}
	*/
	
	if (EODPlayerOwner->IsUsingAnySkill() && EODPlayerOwner->SkillAllowsMovement())
	{
		FSkillTableRow* ActiveSkill = EODPlayerOwner->GetCurrentActiveSkill();
		if (ActiveSkill)
		{
			UAnimMontage* FullBodySkillMontage = ActiveSkill->AnimMontage.Get();
			UAnimMontage* UpperBodySkillMontage = ActiveSkill->UpperBodyAnimMontage.Get();
			FName MontageSection = ActiveSkill->SkillStartMontageSectionName;

			if (FullBodySkillMontage && UpperBodySkillMontage)
			{
				float ForwardAxisValue = EODPlayerOwner->InputComponent->GetAxisValue(FName("MoveForward"));
				float RightAxisValue = EODPlayerOwner->InputComponent->GetAxisValue(FName("MoveRight"));

				if (EODPlayerOwner->SkillHasDirectionalAnimations())
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
}

void UPlayerAnimInstance::NativeUninitializeAnimation()
{
}

bool UPlayerAnimInstance::IsBlocking() const
{
	if (EODPlayerOwner)
	{
		return EODPlayerOwner->IsBlocking();
	}

	return false;
}

bool UPlayerAnimInstance::IsFastRunning() const
{
	if (EODPlayerOwner)
	{
		return EODPlayerOwner->IsFastRunning();
	}

	return false;
}

ECharMovementDirection UPlayerAnimInstance::GetIWRCharMovementDir() const
{
	if (EODPlayerOwner)
	{
		return EODPlayerOwner->GetCharacterMovementDirection();
	}

	return ECharMovementDirection::None;
}

float UPlayerAnimInstance::GetMovementSpeed() const
{
	if (EODPlayerOwner)
	{
		return EODPlayerOwner->GetVelocity().Size();
	}

	return 0.f;
}

float UPlayerAnimInstance::GetBlockMovementDirectionYaw() const
{
	if (EODPlayerOwner)
	{
		return EODPlayerOwner->BlockMovementDirectionYaw;
	}

	return 0.f;
}

bool UPlayerAnimInstance::IsPCTryingToMove() const
{
	if (EODPlayerOwner)
	{
		return EODPlayerOwner->IsPCTryingToMove();
	}

	return false;
}

EWeaponType UPlayerAnimInstance::GetWeaponAnimationType() const
{
	if (EODPlayerOwner && !EODPlayerOwner->IsWeaponSheathed())
	{
		return EODPlayerOwner->GetEquippedWeaponType();
	}

	return EWeaponType::None;
}

void UPlayerAnimInstance::HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(EODPlayerOwner))
	{
		EODPlayerOwner->OnMontageBlendingOut(AnimMontage, bInterrupted);
	}

	/*
	if(!bInterrupted)
	{
		if (AnimMontage == EODPlayerOwner->GetActiveAnimationReferences()->AnimationMontage_Jump)
		{
			// EODPlayerOwner->GetCharacterState() == ECharacterState::IdleWalkRun;
		}
		else if (AnimMontage == EODPlayerOwner->GetActiveAnimationReferences()->AnimationMontage_Dodge)
		{
			// EODPlayerOwner->GetCharacterState() == ECharacterState::IdleWalkRun;
			// @todo
			// EODPlayerOwner-> UpdateMovementAnimation();
		}
		else
		{
			// EODPlayerOwner->GetCharacterState() == ECharacterState::IdleWalkRun;
		}
	}
	*/
}

void UPlayerAnimInstance::HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (EODPlayerOwner)
	{
		EODPlayerOwner->OnMontageEnded(AnimMontage, bInterrupted);
	}
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
