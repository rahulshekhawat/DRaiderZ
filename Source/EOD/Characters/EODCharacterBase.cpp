// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/EODCharacterBase.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/AnimInstances/CharAnimInstance.h"
#include "EOD/Interactives/InteractionInterface.h"
#include "EOD/Statics/EODBlueprintFunctionLibrary.h"

#include "EOD/Player/EODPlayerController.h"
#include "EOD/AI/EODAIControllerBase.h"
#include "EOD/Characters/Components/SkillsComponent.h"
#include "EOD/Characters/Components/StatsComponentBase.h"

#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


AEODCharacterBase::AEODCharacterBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);

	// Initialize Stats Component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UStatsComponentBase>(this, FName("Character Stats Component"));
	SkillsComponent = ObjectInitializer.CreateDefaultSubobject<USkillsComponent>(this, FName("Skills Component"));

	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, FName("Camera Boom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->AddLocalOffset(FVector(0.f, 0.f, 60.f));

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraZoomRate = 15;
	CameraArmMinimumLength = 50;
	CameraArmMaximumLength = 500;

	DefaultWalkSpeed = 400.f;
	DefaultRunSpeed = 600.f;
	DefaultWalkSpeedWhileBlocking = 150.f;

	InteractionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, FName("Interaction Sphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(150.f);

	// No need to enable interaction sphere unless the character is possessed by player controller
	DisableInteractionSphere();

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

	if (GetController() && GetController()->IsLocalPlayerController())
	{
		if (bSkillAllowsMovement)
		{
			
		}

		if (IsMoving())
		{
			UpdateMovement(DeltaTime);
		}
	}
}

void AEODCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEODCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, bIsRunning, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bPCTryingToMove, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterMovementDirection, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bCharacterStateAllowsMovement, COND_SkipOwner);
}

void AEODCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void AEODCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController && NewController->IsLocalPlayerController())
	{
		EnableInteractionSphere();
	}
	else
	{
		DisableInteractionSphere();
	}
}

void AEODCharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

float AEODCharacterBase::BP_GetRotationYawFromAxisInput() const
{
	return GetRotationYawFromAxisInput();
}

float AEODCharacterBase::BP_GetControllerRotationYaw() const
{
	return GetControllerRotationYaw();
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
	return CharacterState == ECharacterState::IdleWalkRun || (IsUsingAnySkill() && bSkillAllowsMovement);
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

/*
// void AEODCharacterBase::OnSuccessfulDodge(AEODCharacterBase* AttackInstigator)
void AEODCharacterBase::SuccessfulDodge(AEODCharacterBase* AttackInstigator)
{
	TWeakObjectPtr<AEODCharacterBase> AttackInstigatorWeakPtr(AttackInstigator);
	// OnSuccessfulDodgeEvent.Broadcast(AttackInstigatorWeakPtr);
}

void AEODCharacterBase::OnSuccessfulBlock(AEODCharacterBase * AttackInstigator)
{
	TWeakObjectPtr<AEODCharacterBase> AttackInstigatorWeakPtr(AttackInstigator);
	// OnSuccessfulBlockEvent.Broadcast(AttackInstigatorWeakPtr);
	BlockAttack();
}

void AEODCharacterBase::OnAttackDeflected(AEODCharacterBase * AttackBlocker, bool bSkillIgnoresBlock)
{
	TWeakObjectPtr<AEODCharacterBase> AttackBlockerWeakPtr(AttackBlocker);
	// OnAttackDeflectedEvent.Broadcast(AttackBlockerWeakPtr);

	if (!bSkillIgnoresBlock)
	{
		// PlayAnimationMontage()
	}
}
*/

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

bool AEODCharacterBase::UseSkill_Implementation(FName SkillID)
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

float AEODCharacterBase::GetOrientationYawToActor(AActor* TargetActor)
{
	FVector OrientationVector = TargetActor->GetActorLocation() - GetActorLocation();
	FRotator OrientationRotator = OrientationVector.ToOrientationRotator();
	return OrientationRotator.Yaw;
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

bool AEODCharacterBase::StartDodging()
{
	if (CanDodge())
	{
		// @dodge logic
		return true;
	}

	return false;
}

bool AEODCharacterBase::StopDodging()
{
	if (IsDodging())
	{
		// @stop dodge
	}

	return true;
}

bool AEODCharacterBase::StartBlockingAttacks()
{
	if (CanBlock())
	{
		return true;
	}

	return false;
}

bool AEODCharacterBase::StopBlockingAttacks()
{
	if (IsBlocking())
	{

	}

	return true;
}

void AEODCharacterBase::TriggerInteraction()
{
	// If Character is already interacting
	if (GetCharacterState() == ECharacterState::Interacting)
	{
		UpdateInteraction();
	}
	else
	{
		StartInteraction();
	}
}

void AEODCharacterBase::StartInteraction()
{
}

void AEODCharacterBase::UpdateInteraction()
{
}

void AEODCharacterBase::StopInteraction()
{
}

void AEODCharacterBase::ToggleSheathe()
{

}

void AEODCharacterBase::StartNormalAttacking()
{
}

void AEODCharacterBase::StopNormalAttacking()
{
}

void AEODCharacterBase::UpdateMovement(float DeltaTime)
{
	float DesiredRotationYaw = GetRotationYawFromAxisInput();
	bool bRotatePlayer = DesiredRotationYaw == GetActorRotation().Yaw ? false : true;

	if (bRotatePlayer)
	{
		DeltaRotateCharacterToDesiredYaw(DesiredRotationYaw, DeltaTime);
	}

	// if (GetEODPlayerController())
	if (Controller)
	{
		// float ForwardAxisValue = GetController()->InputComponent->GetAxisValue(FName("MoveForward"));
		float ForwardAxisValue = Controller->InputComponent->GetAxisValue(FName("MoveForward"));
		// float RightAxisValue = GetController()->InputComponent->GetAxisValue(FName("MoveRight"));
		float RightAxisValue = Controller->InputComponent->GetAxisValue(FName("MoveRight"));

		if (ForwardAxisValue < 0)
		{
			float Speed = (DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
			if (GetCharacterMovement()->MaxWalkSpeed != Speed)
			{
				SetWalkSpeed(Speed);
			}
		}
		else
		{
			float Speed = DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier();
			if (GetCharacterMovement()->MaxWalkSpeed != Speed)
			{
				SetWalkSpeed(Speed);
			}
		}

		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0 && CharacterMovementDirection != ECharMovementDirection::R)
			{
				SetCharacterMovementDirection(ECharMovementDirection::R);
			}
			else if (RightAxisValue < 0 && CharacterMovementDirection != ECharMovementDirection::L)
			{
				SetCharacterMovementDirection(ECharMovementDirection::L);
			}
		}
		else
		{
			if (ForwardAxisValue > 0 && CharacterMovementDirection != ECharMovementDirection::F)
			{
				SetCharacterMovementDirection(ECharMovementDirection::F);
			}
			else if (ForwardAxisValue < 0 && CharacterMovementDirection != ECharMovementDirection::B)
			{
				SetCharacterMovementDirection(ECharMovementDirection::B);
			}
		}
	}
}

void AEODCharacterBase::Server_SetIsRunning_Implementation(bool bValue)
{
	SetIsRunning(bValue);
}

bool AEODCharacterBase::Server_SetIsRunning_Validate(bool bValue)
{
	return true;
}

void AEODCharacterBase::Server_SetCharacterStateAllowsMovement_Implementation(bool bNewValue)
{
	SetCharacterStateAllowsMovement(bNewValue);
}

bool AEODCharacterBase::Server_SetCharacterStateAllowsMovement_Validate(bool bNewValue)
{
	return true;
}

void AEODCharacterBase::Server_SetPCTryingToMove_Implementation(bool bNewValue)
{
	SetPCTryingToMove(bNewValue);
}

bool AEODCharacterBase::Server_SetPCTryingToMove_Validate(bool bNewValue)
{
	return true;
}

void AEODCharacterBase::Server_SetCharMovementDir_Implementation(ECharMovementDirection NewDirection)
{
	SetCharacterMovementDirection(NewDirection);
}

bool AEODCharacterBase::Server_SetCharMovementDir_Validate(ECharMovementDirection NewDirection)
{
	return true;
}
