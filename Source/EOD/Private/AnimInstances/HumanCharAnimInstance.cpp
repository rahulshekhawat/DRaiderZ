// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HumanCharAnimInstance.h"
#include "HumanCharacter.h"

UHumanCharAnimInstance::UHumanCharAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MasterStateMachine_AnimationsBlendTime = 0.f;
	IdleWalkRun_AnimationsBlendTime = 0.2f;
	CurrentWeaponType = EWeaponType::None;

	bJumpAnimationPlayingLocally = false;
	bJumpLandAnimationPlayingLocally = false;
}

void UHumanCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	HumanOwner = TryGetPawnOwner() ? Cast<AHumanCharacter>(TryGetPawnOwner()) : nullptr;
}

void UHumanCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(HumanOwner))
	{
		if (!IsValid((HumanOwner = TryGetPawnOwner() ? Cast<AHumanCharacter>(TryGetPawnOwner()) : nullptr)))
		{
			return;
		}
	}

	MovementSpeed = HumanOwner->GetVelocity().Size();
	CharacterMovementDirection = HumanOwner->GetCharacterMovementDirection();
	bIsBlocking = HumanOwner->IsBlocking();
	bIsRunning = HumanOwner->IsRunning();
	BlockMovementDirectionYaw = HumanOwner->GetBlockMovementDirectionYaw();
	bPCTryingToMove = HumanOwner->IsPCTryingToMove();

	CurrentWeaponType = HumanOwner->IsWeaponSheathed() ? EWeaponType::None : HumanOwner->GetEquippedWeaponType();

	FPlayerAnimationReferencesTableRow* AnimationReferences = HumanOwner->GetActiveAnimationReferences();
	if (!AnimationReferences)
	{
		return;
	}

	//~ @todo return if player is dead.

	bool bIsFalling = HumanOwner->GetCharacterMovement() && HumanOwner->GetCharacterMovement()->IsFalling();
	bool bCanJumpAnimationPlay = HumanOwner->IsIdleOrMoving() || HumanOwner->IsJumping() || HumanOwner->IsBlocking();
	if (bIsFalling && !bJumpAnimationPlayingLocally && bCanJumpAnimationPlay)
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage)
		{
			Montage_Play(JumpMontage);
			HumanOwner->OnJumpAnimationStart();
			bJumpAnimationPlayingLocally = true;

			if (CurrentWeaponType == EWeaponType::Dagger)
			{
				if (HumanOwner->ForwardAxisValue >= 0)
				{
					Montage_JumpToSection(UCharacterLibrary::SectionName_ForwardJumpStart);
				}
				else
				{
					Montage_JumpToSection(UCharacterLibrary::SectionName_BackwardJumpStart);
				}
			}
		}
	}
	else if (!bIsFalling && bJumpAnimationPlayingLocally)
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (!bJumpLandAnimationPlayingLocally)
		{
			if (CurrentWeaponType != EWeaponType::Dagger)
			{
				Montage_JumpToSection(UCharacterLibrary::SectionName_JumpEnd, JumpMontage);
			}
			else
			{
				FName CurrentSection = Montage_GetCurrentSection(JumpMontage);
				if (CurrentSection == UCharacterLibrary::SectionName_ForwardJumpLoop)
				{
					Montage_JumpToSection(UCharacterLibrary::SectionName_ForwardJumpEnd, JumpMontage);
				}
				else if (CurrentSection == UCharacterLibrary::SectionName_BackwardJumpLoop)
				{
					Montage_JumpToSection(UCharacterLibrary::SectionName_BackwardJumpEnd, JumpMontage);
				}
				else
				{
					Montage_JumpToSection(UCharacterLibrary::SectionName_ForwardJumpEnd, JumpMontage);
				}
			}
			bJumpLandAnimationPlayingLocally = true;
		}
	}

	if (HumanOwner->IsSwitchingWeapon())
	{
		// Movable montage should play based on whether PC is trying to move or not
		if (bPCTryingToMove != bMovableMontagePlaying)
		{
			UAnimMontage* FullBodySwitchMontage = HumanOwner->GetEquippedWeaponAnimationReferences()->WeaponSwitchFullBody.Get();
			UAnimMontage* UpperBodySwitchMontage = HumanOwner->GetEquippedWeaponAnimationReferences()->WeaponSwitchUpperBody.Get();

			FName MontageSection = HumanOwner->IsWeaponSheathed() ? UCharacterLibrary::SectionName_SheatheWeapon : UCharacterLibrary::SectionName_UnsheatheWeapon;

			if (FullBodySwitchMontage && UpperBodySwitchMontage)
			{
				DoSeamlessTransitionBetweenStillOrMovingMontage(FullBodySwitchMontage, UpperBodySwitchMontage, MontageSection, true);
			}
		}
	}

	if (HumanOwner->IsDead())
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

	//~ @todo
	/*
	if (HumanOwner->IsUsingAnySkill() && HumanOwner->SkillAllowsMovement())
	{
		FSkillTableRow* ActiveSkill = HumanOwner->GetCurrentActiveSkill();
		if (ActiveSkill)
		{
			UAnimMontage* FullBodySkillMontage = ActiveSkill->AnimMontage.Get();
			UAnimMontage* UpperBodySkillMontage = ActiveSkill->UpperBodyAnimMontage.Get();
			FName MontageSection = ActiveSkill->SkillStartMontageSectionName;

			if (FullBodySkillMontage && UpperBodySkillMontage)
			{
				float ForwardAxisValue = HumanOwner->InputComponent->GetAxisValue(FName("MoveForward"));
				float RightAxisValue = HumanOwner->InputComponent->GetAxisValue(FName("MoveRight"));

				if (HumanOwner->SkillHasDirectionalAnimations())
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
	*/
}

void UHumanCharAnimInstance::NativePostEvaluateAnimation()
{
}

void UHumanCharAnimInstance::NativeUninitializeAnimation()
{
}

void UHumanCharAnimInstance::HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	Super::HandleMontageBlendingOut(AnimMontage, bInterrupted);

	if (!HumanOwner)
	{
		return;
	}

	FPlayerAnimationReferencesTableRow* AnimationReferences = HumanOwner->GetActiveAnimationReferences();
	UAnimMontage* JumpMontage = AnimationReferences ? AnimationReferences->Jump.Get() : nullptr;
	if (JumpMontage && JumpMontage == AnimMontage)
	{
		bJumpAnimationPlayingLocally = false;
		bJumpLandAnimationPlayingLocally = false;
		HumanOwner->OnJumpAnimationFinish();
	}
}

void UHumanCharAnimInstance::HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	Super::HandleMontageEnded(AnimMontage, bInterrupted);
}
