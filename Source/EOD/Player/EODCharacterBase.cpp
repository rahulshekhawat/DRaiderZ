// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "Player/EODCharacterBase.h"
#include "Core/EODPreprocessors.h"
#include "Player/CharAnimInstance.h"
#include "Components/StatsComponentBase.h"
#include "Statics/EODBlueprintFunctionLibrary.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


AEODCharacterBase::AEODCharacterBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Stats Component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UStatsComponentBase>(this, FName("Character Stats Component"));
	
	// Initialize variables
	CharacterState = ECharacterState::IdleWalkRun;
	bGodMode = false;
	TargetSwitchDuration = 0.1f;

	// MaxNumberOfSkills = 30;
	DodgeStaminaCost = 20;
	DodgeImmunityTriggerDelay = 0.1f;
	DodgeImmunityDuration = 0.4;
	DamageBlockTriggerDelay = 0.2f;

	Faction = EFaction::Player;
}

void AEODCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEODCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterState, COND_SkipOwner);

}

void AEODCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

bool AEODCharacterBase::BP_IsDead() const
{
	return IsDead();
}

bool AEODCharacterBase::BP_HasBeenHit() const
{
	return HasBeenHit();
}

bool AEODCharacterBase::CanMove() const
{
	// Mobs can only move in IdleWalkRun state
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanJump() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanDodge() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanBlock() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanRespawn() const
{
	return false;
}

bool AEODCharacterBase::CanNormalAttack() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanUseAnySkill() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanUseSkill(FSkillTableRow * Skill)
{
	return false;
}

bool AEODCharacterBase::BP_NeedsHealing() const
{
	return NeedsHealing();
}

bool AEODCharacterBase::IsHealing() const
{
	return false;
}

bool AEODCharacterBase::CCEInterrupt_Implementation(const float BCAngle)
{
	return false;
}

bool AEODCharacterBase::CCEStun_Implementation(const float Duration)
{
	return false;
}

void AEODCharacterBase::CCERemoveStun_Implementation()
{
}

bool AEODCharacterBase::CCEFreeze_Implementation(const float Duration)
{
	return false;
}

void AEODCharacterBase::CCEUnfreeze_Implementation()
{
}

bool AEODCharacterBase::CCEKnockdown_Implementation(const float Duration)
{
	return false;
}

void AEODCharacterBase::CCEEndKnockdown_Implementation()
{
}

bool AEODCharacterBase::CCEFlinch_Implementation(const float BCAngle)
{
	return false;
}

bool AEODCharacterBase::CCEKnockback_Implementation(const float Duration, const FVector & ImpulseDirection)
{
	return false;
}

void AEODCharacterBase::InitiateDeathSequence_Implementation()
{
}

bool AEODCharacterBase::Stun(const float Duration)
{
	return false;
}

void AEODCharacterBase::EndStun()
{
}

bool AEODCharacterBase::Freeze(const float Duration)
{
	return false;
}

void AEODCharacterBase::EndFreeze()
{
}

bool AEODCharacterBase::Knockdown(const float Duration)
{
	return false;
}

void AEODCharacterBase::EndKnockdown()
{
}

bool AEODCharacterBase::Knockback(const float Duration, const FVector & ImpulseDirection)
{
	return false;
}

void AEODCharacterBase::BlockAttack()
{
}

void AEODCharacterBase::EnableiFrames(float Duration)
{
	bActiveiFrames = true;

	if (Duration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DodgeImmunityTimerHandle, this, &AEODCharacterBase::DisableiFrames, Duration, false);
	}
}

void AEODCharacterBase::DisableiFrames()
{
	bActiveiFrames = false;
}

void AEODCharacterBase::EnableDamageBlocking()
{
	bBlockingDamage = true;
}

void AEODCharacterBase::DisableDamageBlocking()
{
	bBlockingDamage = false;
	// Clear block damage timer just in case it is still active
	GetWorld()->GetTimerManager().ClearTimer(BlockTimerHandle); 
}

void AEODCharacterBase::OnSuccessfulDodge(AEODCharacterBase* AttackInstigator)
{
	TWeakObjectPtr<AEODCharacterBase> AttackInstigatorWeakPtr(AttackInstigator);
	OnSuccessfulDodgeEvent.Broadcast(AttackInstigatorWeakPtr);
}

void AEODCharacterBase::OnSuccessfulBlock(AEODCharacterBase * AttackInstigator)
{
	TWeakObjectPtr<AEODCharacterBase> AttackInstigatorWeakPtr(AttackInstigator);
	OnSuccessfulBlockEvent.Broadcast(AttackInstigatorWeakPtr);
	BlockAttack();
}

void AEODCharacterBase::OnAttackDeflected(AEODCharacterBase * AttackBlocker, bool bSkillIgnoresBlock)
{
	TWeakObjectPtr<AEODCharacterBase> AttackBlockerWeakPtr(AttackBlocker);
	OnAttackDeflectedEvent.Broadcast(AttackBlockerWeakPtr);

	if (!bSkillIgnoresBlock)
	{
		// PlayAnimationMontage()
	}
}

bool AEODCharacterBase::BP_GetInCombat() const
{
	return GetInCombat();
}

void AEODCharacterBase::BP_SetCharacterState(const ECharacterState NewState)
{
	SetCharacterState(NewState);
}

ECharacterState AEODCharacterBase::BP_GetCharacterState() const
{
	return GetCharacterState();
}

void AEODCharacterBase::SetWalkSpeed(const float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	if (Role < ROLE_Authority)
	{
		Server_SetWalkSpeed(WalkSpeed);
	}
}

void AEODCharacterBase::SetCharacterRotation(const FRotator NewRotation)
{
	GetCharacterMovement()->FlushServerMoves();
	SetActorRotation(NewRotation);

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterRotation(NewRotation);
	}
}

void AEODCharacterBase::SetUseControllerRotationYaw(const bool bNewBool)
{
	bUseControllerRotationYaw = bNewBool;

	if (Role < ROLE_Authority)
	{
		Server_SetUseControllerRotationYaw(bNewBool);
	}
}

bool AEODCharacterBase::UseSkill(FName SkillID)
{
	return false;
}

EEODTaskStatus AEODCharacterBase::CheckSkillStatus(FName SkillID)
{
	return EEODTaskStatus();
}

FName AEODCharacterBase::GetMostWeightedMeleeSkillID(AEODCharacterBase const * const TargetCharacter) const
{
	return FName();
}

FName AEODCharacterBase::BP_GetCurrentActiveSkillID() const
{
	return GetCurrentActiveSkillID();
}

void AEODCharacterBase::BP_SetCurrentActiveSkillID(FName SkillID)
{
	SetCurrentActiveSkillID(SkillID);
}

FLastUsedSkillInfo& AEODCharacterBase::BP_GetLastUsedSkill()
{
	return GetLastUsedSkill();
}

void AEODCharacterBase::ApplyStatusEffect(const UStatusEffectBase * StatusEffect)
{
	// @todo definition
}

void AEODCharacterBase::RemoveStatusEffect(const UStatusEffectBase * StatusEffect)
{
	// @todo definition
}

void AEODCharacterBase::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::BP_PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

void AEODCharacterBase::SetNextMontageSection(FName CurrentSection, FName NextSection)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}

	Server_SetNextMontageSection(CurrentSection, NextSection);
}

bool AEODCharacterBase::DeltaRotateCharacterToDesiredYaw(float DesiredYaw, float DeltaTime, float Precision, float RotationRate)
{
	float CurrentYaw = GetActorRotation().Yaw;
	float YawDiff = FMath::FindDeltaAngleDegrees(CurrentYaw, DesiredYaw);
	if (FMath::Abs(YawDiff) < Precision)
	{
		return true;
	}

	float Multiplier = YawDiff / FMath::Abs(YawDiff);
	float RotateBy = Multiplier * RotationRate * DeltaTime;
	if (FMath::Abs(YawDiff) <= FMath::Abs(RotateBy))
	{
		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));
		return true;
	}
	else if (FMath::Abs(YawDiff) <= FMath::Abs(RotateBy) + Precision)
	{
		SetCharacterRotation(FRotator(0.f, CurrentYaw + RotateBy, 0.f));
		return true;
	}
	else
	{
		SetCharacterRotation(FRotator(0.f, CurrentYaw + RotateBy, 0.f));
		return false;
	}
}

void AEODCharacterBase::Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase * InstigatingChar)
{
	if (bGodMode || IsDead())
	{
		// cannot die
		return;
	}

	if (CauseOfDeath == ECauseOfDeath::ZeroHP)
	{

	}
	else
	{
		// Set current hp to 0
		StatsComp->ModifyBaseHealth(-StatsComp->GetMaxHealth());
		SetCharacterState(ECharacterState::Dead);

		// @todo play death animation and death sound
	}
}

void AEODCharacterBase::TurnOnTargetSwitch()
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	GetWorld()->GetTimerManager().SetTimer(TargetSwitchTimerHandle, this, &AEODCharacterBase::TurnOffTargetSwitch, TargetSwitchDuration, false);
}

void AEODCharacterBase::TurnOffTargetSwitch()
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
}

void AEODCharacterBase::OnRep_CharacterState(ECharacterState OldState)
{
	//~ @todo : Cleanup old state
}

void AEODCharacterBase::Server_SetCharacterState_Implementation(ECharacterState NewState)
{
	SetCharacterState(NewState);
}

bool AEODCharacterBase::Server_SetCharacterState_Validate(ECharacterState NewState)
{
	return true;
}

void AEODCharacterBase::Server_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{

	Multicast_SetNextMontageSection(CurrentSection, NextSection);
}

bool AEODCharacterBase::Server_SetNextMontageSection_Validate(FName CurrentSection, FName NextSection)
{
	return true;
}

void AEODCharacterBase::Server_SetUseControllerRotationYaw_Implementation(bool bNewBool)
{
	SetUseControllerRotationYaw(bNewBool);
}

bool AEODCharacterBase::Server_SetUseControllerRotationYaw_Validate(bool bNewBool)
{
	return true;
}

void AEODCharacterBase::Server_SetCharacterRotation_Implementation(FRotator NewRotation)
{
	SetCharacterRotation(NewRotation);
}

bool AEODCharacterBase::Server_SetCharacterRotation_Validate(FRotator NewRotation)
{
	return true;
}

void AEODCharacterBase::Server_SetWalkSpeed_Implementation(float WalkSpeed)
{
	SetWalkSpeed(WalkSpeed);
}

bool AEODCharacterBase::Server_SetWalkSpeed_Validate(float WalkSpeed)
{
	return true;
}

void AEODCharacterBase::Multicast_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{
	FString Message = FString("Multi cast called");
	UKismetSystemLibrary::PrintString(this, Message, true, false);

	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}
}

void AEODCharacterBase::Server_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	MultiCast_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

bool AEODCharacterBase::Server_PlayAnimationMontage_Validate(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	return true;
}

void AEODCharacterBase::MultiCast_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay);
		CharacterState = NewState;
	}
}
