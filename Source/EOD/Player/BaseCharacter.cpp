// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BaseCharacter.h"
#include "CharAnimInstance.h"
#include "Components/BaseStatsComponent.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Stats Component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UBaseStatsComponent>(this, FName("Character Stats Component"));
	
	// Initialize variables
	CharacterState = ECharacterState::IdleWalkRun;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABaseCharacter, CharacterState, COND_SkipOwner);

}

bool ABaseCharacter::IsAlive() const
{
	return StatsComp->GetCurrentHealth() > 0;
}

bool ABaseCharacter::IsDead() const
{
	return StatsComp->GetCurrentHealth() <= 0;
}

bool ABaseCharacter::IsIdle() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0);
}

bool ABaseCharacter::IsMoving() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() != 0);
}

bool ABaseCharacter::IsJumping() const
{
	return CharacterState == ECharacterState::Jumping;
}

bool ABaseCharacter::IsBlocking() const
{
	return CharacterState == ECharacterState::Blocking;
}

bool ABaseCharacter::IsCastingSpell() const
{
	return CharacterState == ECharacterState::CastingSpell;
}

bool ABaseCharacter::IsNormalAttacking() const
{
	return CharacterState == ECharacterState::Attacking;
}

bool ABaseCharacter::IsDodging() const
{
	return CharacterState == ECharacterState::Dodging;
}

bool ABaseCharacter::NeedsHeal() const
{
	return StatsComp->IsLowOnHealth();
}

void ABaseCharacter::SetCharacterState(ECharacterState NewState)
{
	CharacterState = NewState;

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterState(NewState);
	}
}

void ABaseCharacter::Server_SetCharacterState_Implementation(ECharacterState NewState)
{
	SetCharacterState(NewState);
}

bool ABaseCharacter::Server_SetCharacterState_Validate(ECharacterState NewState)
{
	return true;
}

void ABaseCharacter::SetWalkSpeed(float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	if (Role < ROLE_Authority)
	{
		Server_SetWalkSpeed(WalkSpeed);
	}
}

void ABaseCharacter::SetCharacterRotation(FRotator NewRotation)
{
	GetCharacterMovement()->FlushServerMoves();
	SetActorRotation(NewRotation);

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterRotation(NewRotation);
	}
}

void ABaseCharacter::SetUseControllerRotationYaw(bool bNewBool)
{
	bUseControllerRotationYaw = bNewBool;

	if (Role < ROLE_Authority)
	{
		Server_SetUseControllerRotationYaw(bNewBool);
	}
}

void ABaseCharacter::SetNextMontageSection(FName CurrentSection, FName NextSection)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}

	Server_SetNextMontageSection(CurrentSection, NextSection);
}

void ABaseCharacter::Server_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{

	Multicast_SetNextMontageSection(CurrentSection, NextSection);
}

bool ABaseCharacter::Server_SetNextMontageSection_Validate(FName CurrentSection, FName NextSection)
{
	return true;
}

void ABaseCharacter::Server_SetUseControllerRotationYaw_Implementation(bool bNewBool)
{
	SetUseControllerRotationYaw(bNewBool);
}

bool ABaseCharacter::Server_SetUseControllerRotationYaw_Validate(bool bNewBool)
{
	return true;
}

void ABaseCharacter::Server_SetCharacterRotation_Implementation(FRotator NewRotation)
{
	SetCharacterRotation(NewRotation);
}

bool ABaseCharacter::Server_SetCharacterRotation_Validate(FRotator NewRotation)
{
	return true;
}

void ABaseCharacter::Server_SetWalkSpeed_Implementation(float WalkSpeed)
{
	SetWalkSpeed(WalkSpeed);
}

bool ABaseCharacter::Server_SetWalkSpeed_Validate(float WalkSpeed)
{
	return true;
}

void ABaseCharacter::Multicast_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{
	FString Message = FString("Multi cast called");
	UKismetSystemLibrary::PrintString(this, Message, true, false);

	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}
}

void ABaseCharacter::PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay);
		CharacterState = NewState;
	}

	Server_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

void ABaseCharacter::Server_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	MultiCast_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

bool ABaseCharacter::Server_PlayAnimationMontage_Validate(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	return true;
}

void ABaseCharacter::MultiCast_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay);
		CharacterState = NewState;
	}
}

bool ABaseCharacter::CanMove() const
{
	// Mobs can only move in IdleWalkRun state
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool ABaseCharacter::CanJump() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool ABaseCharacter::CanBlock() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool ABaseCharacter::CanRespawn() const
{
	return false;
}

bool ABaseCharacter::CanNormalAttack() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool ABaseCharacter::CanDodge() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

/*
void ABaseCharacter::AddStatusEffectVisuals(FStatusEffectInfo StatusEffectInfo)
{
	// @todo definition
}

void ABaseCharacter::RemoveStatusEffectVisuals(FStatusEffectInfo StatusEffectInfo)
{
	// @todo definition
}
*/

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
