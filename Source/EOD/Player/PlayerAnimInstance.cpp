// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "Core/EODPreprocessors.h"
#include "Statics/CharacterLibrary.h"

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

	if (OwningPlayer->GetMovementComponent()->IsFalling())
	{
		UAnimMontage* JumpMontage = AnimationReferences->Jump.Get();
		if (JumpMontage && !Montage_IsPlaying(JumpMontage))
		{
			// If some montage is already playing
			if (Montage_IsPlaying(nullptr))
			{
				UAnimMontage* FlinchMontage = AnimationReferences->Flinch.Get();
				UAnimMontage* BlockAttackMontage = AnimationReferences->BlockAttack.Get();

				if ((FlinchMontage && Montage_IsPlaying(FlinchMontage)) || (BlockAttackMontage && Montage_IsPlaying(FlinchMontage)))
				{
					OwningPlayer->SetCharacterState(ECharacterState::Jumping);
					Montage_Play(JumpMontage);
					return;
				}
			}
			// If no montage is playing
			else
			{
				OwningPlayer->SetCharacterState(ECharacterState::Jumping);
				Montage_Play(JumpMontage);
				return;
			}
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
