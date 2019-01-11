// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/EODCharacterBase.h"
#include "EOD/AnimInstances/CharAnimInstance.h"
#include "EOD/Interactives/InteractionInterface.h"
#include "EOD/Statics/EODBlueprintFunctionLibrary.h"
#include "EOD/Player/EODPlayerController.h"
#include "EOD/AI/EODAIControllerBase.h"
#include "EOD/UI/HUDWidget.h"
#include "EOD/Characters/Components/SkillsComponent.h"
#include "EOD/Characters/Components/StatsComponentBase.h"
#include "EOD/Characters/Components/GameplaySkillsComponent.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"

#include "UnrealNetwork.h"
#include "Camera/CameraComponent.h"


FName AEODCharacterBase::CameraComponentName(TEXT("Camera"));
FName AEODCharacterBase::SpringArmComponentName(TEXT("Camera Boom"));
FName AEODCharacterBase::CharacterStatsComponentName(TEXT("Character Stats"));
FName AEODCharacterBase::GameplaySkillsComponentName(TEXT("Skill Manager"));
FName AEODCharacterBase::InteractionSphereComponentName(TEXT("Interaction Sphere"));

AEODCharacterBase::AEODCharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UEODCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterStatsComponent = ObjectInitializer.CreateDefaultSubobject<UStatsComponentBase>(this, AEODCharacterBase::CharacterStatsComponentName);
	SkillManager = ObjectInitializer.CreateDefaultSubobject<UGameplaySkillsComponent>(this, AEODCharacterBase::GameplaySkillsComponentName);

	CameraBoomComponent = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, AEODCharacterBase::SpringArmComponentName);
	if (CameraBoomComponent)
	{
		CameraBoomComponent->bUsePawnControlRotation = true;
		CameraBoomComponent->SetupAttachment(RootComponent);
		CameraBoomComponent->AddLocalOffset(FVector(0.f, 0.f, 60.f));
	}

	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, AEODCharacterBase::CameraComponentName);
	if (CameraComponent)
	{
		CameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);
	}

	InteractionSphereComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, AEODCharacterBase::InteractionSphereComponentName);
	if (InteractionSphereComponent)
	{
		InteractionSphereComponent->SetupAttachment(RootComponent);
		InteractionSphereComponent->SetSphereRadius(150.f);
		// No need to enable interaction sphere unless the character is possessed by player controller
		InteractionSphereComponent->Deactivate();
		InteractionSphereComponent->SetCollisionProfileName(CollisionProfileNames::NoCollision);
	}

	SetReplicates(true);
	SetReplicateMovement(true);
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetIsReplicated(true);
	}

	CameraZoomRate = 15;
	CameraArmMinimumLength = 50;
	CameraArmMaximumLength = 500;

	DefaultWalkSpeed = 400.f;
	DefaultRunSpeed = 600.f;
	DefaultWalkSpeedWhileBlocking = 150.f;

	// Initialize variables
	CharacterState = ECharacterState::IdleWalkRun;
	bGodMode = false;
	TargetSwitchDuration = 0.1f;

	// By default the weapon should be sheathed
	bWeaponSheathed = true;

	// MaxNumberOfSkills = 30;
	DodgeStaminaCost = 30;
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
		// If block key is pressed but the character is not blocking
		if (bGuardKeyPressed && !IsGuardActive() && CanGuardAgainstAttacks())
		{
			ActivateGuard();
		}
		// If block is not pressed but character is blocking
		else if (!bGuardKeyPressed && IsGuardActive())
		{
			DeactivateGuard();
		}

		if (bNormalAttackKeyPressed && !IsNormalAttacking() && CanNormalAttack())
		{
			StartNormalAttack();
		}
		else if (bNormalAttackKeyPressed && IsNormalAttacking())
		{
			UpdateNormalAttackState(DeltaTime);
		}

		if (GetCharacterMovement()->IsFalling() && bCharacterStateAllowsMovement)
		{
			SetCharacterStateAllowsMovement(false);
		}

		// If the character is either idle or moving, or is in a state that allows movement except guard
		if (IsIdleOrMoving() || (bCharacterStateAllowsMovement && !IsGuardActive()))
		{
			UpdatePCTryingToMove();
			UpdateCharacterMovementDirection();
			UpdateDesiredYawFromAxisInput();
			UpdateMovementState(DeltaTime);
		}
		else if (IsGuardActive())
		{
			UpdateGuardState(DeltaTime);
		}
	}
}

void AEODCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, bIsRunning, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bGuardActive, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bWeaponSheathed, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bPCTryingToMove, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, BlockMovementDirectionYaw, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterMovementDirection, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bCharacterStateAllowsMovement, COND_SkipOwner);
}

void AEODCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Intentional additional call to BindUIDelegates (another in Restart())
	BindUIDelegates();
}

void AEODCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// @todo - Enable interaction sphere on client.
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

void AEODCharacterBase::Restart()
{
	Super::Restart();

	// Intentional additional call to BindUIDelegates (another in BeginPlay())
	BindUIDelegates();
}

float AEODCharacterBase::BP_GetRotationYawFromAxisInput() const
{
	return GetRotationYawFromAxisInput();
}

float AEODCharacterBase::BP_GetControllerRotationYaw() const
{
	return GetControllerRotationYaw();
}

void AEODCharacterBase::BP_TriggeriFrames(float Duration, float Delay)
{
	TriggeriFrames(Duration, Delay);
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

bool AEODCharacterBase::CanGuardAgainstAttacks() const
{
	return (IsIdleOrMoving() || IsNormalAttacking()) && !(IsWeaponSheathed());
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

void AEODCharacterBase::BindUIDelegates()
{
	if (GetController() && GetController()->IsLocalPlayerController())
	{
		AEODPlayerController* PC = Cast<AEODPlayerController>(Controller);
		if (IsValid(PC) && IsValid(PC->GetHUDWidget()))
		{
			UStatusIndicatorWidget* StatusIndicatorWidget = PC->GetHUDWidget()->GetStatusIndicatorWidget();
			if (IsValid(CharacterStatsComponent) && IsValid(StatusIndicatorWidget))
			{
				CharacterStatsComponent->OnHealthChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateHealthBar);
				CharacterStatsComponent->OnManaChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateManaBar);
				CharacterStatsComponent->OnStaminaChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateStaminaBar);
			}

			USkillBarWidget* SkillBarWidget = PC->GetHUDWidget()->GetSkillBarWidget();
			if (IsValid(SkillManager) && IsValid(SkillBarWidget))
			{
				SkillBarWidget->UpdateSkillBarLayout(SkillManager->GetSkillBarLayout());
				SkillBarWidget->OnNewSkillAdded.AddUniqueDynamic(SkillManager, &UGameplaySkillsComponent::AddNewSkill);
			}
		}
	}
}

void AEODCharacterBase::UnbindUIDelegates()
{
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

bool AEODCharacterBase::BP_IsInCombat() const
{
	return IsInCombat();
}

void AEODCharacterBase::BP_SetCharacterState(const ECharacterState NewState)
{
	SetCharacterState(NewState);
}

ECharacterState AEODCharacterBase::BP_GetCharacterState() const
{
	return GetCharacterState();
}

void AEODCharacterBase::BP_SetWalkSpeed(const float WalkSpeed)
{
	SetWalkSpeed(WalkSpeed);
}

void AEODCharacterBase::BP_SetCharacterRotation(const FRotator NewRotation)
{
	SetCharacterRotation(NewRotation);
}

void AEODCharacterBase::BP_SetUseControllerRotationYaw(const bool bNewBool)
{
	SetUseControllerRotationYaw(bNewBool);
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
	PrintToScreen(this, FString("Rotating"));
	float CurrentYaw = GetActorRotation().Yaw;
	if (FMath::IsNearlyEqual(CurrentYaw, DesiredYaw, Precision))
	{
		return true;
	}

	float DeltaRotationYaw = (RotationRate >= 0.f) ? (RotationRate * DeltaTime) : 360.f;
	float NewRotationYaw = FMath::FixedTurn(CurrentYaw, DesiredYaw, DeltaRotationYaw);
	SetCharacterRotation(FRotator(0.f, NewRotationYaw, 0.f));

	if (FMath::IsNearlyEqual(CurrentYaw, NewRotationYaw, Precision))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AEODCharacterBase::Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase* InstigatingChar)
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
		GetCharacterStatsComponent()->ModifyBaseHealth(-GetCharacterStatsComponent()->GetMaxHealth());
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

void AEODCharacterBase::OnRep_WeaponSheathed()
{
	PlayToggleSheatheAnimation();
}

void AEODCharacterBase::OnRep_GuardActive()
{
	if (IsGuardActive())
	{
		EnableCharacterGuard();
	}
	else
	{
		DisableCharacterGuard();
	}
	// ActivateGuard();
}

void AEODCharacterBase::OnRep_CharacterState(ECharacterState OldState)
{
	//~ @todo : Cleanup old state
}

void AEODCharacterBase::Client_DisplayTextOnPlayerScreen_Implementation(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition)
{
	if (IsPlayerControlled())
	{
		CreateAndDisplayTextOnPlayerScreen(Message, TextColor, TextPosition);
	}
}

void AEODCharacterBase::Server_StopBlockingDamage_Implementation()
{
	StopBlockingDamage();
}

bool AEODCharacterBase::Server_StopBlockingDamage_Validate()
{
	return true;
}

void AEODCharacterBase::Server_StartBlockingDamage_Implementation(float Delay)
{
	StartBlockingDamage(Delay);
}

bool AEODCharacterBase::Server_StartBlockingDamage_Validate(float Delay)
{
	return true;
}

void AEODCharacterBase::Server_TriggeriFrames_Implementation(float Duration, float Delay)
{
	TriggeriFrames(Duration, Delay);
}

bool AEODCharacterBase::Server_TriggeriFrames_Validate(float Duration, float Delay)
{
	return true;
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
	Multicast_SetCharacterRotation(NewRotation);
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

void AEODCharacterBase::StartDodge()
{
	// @todo
}

void AEODCharacterBase::StopDodge()
{
	// @todo
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

void AEODCharacterBase::EnableCharacterGuard()
{
	// @todo wait for normal attack section to finish before blocking?
	if (IsNormalAttacking())
	{
		StopNormalAttack();
	}
	SetCharacterState(ECharacterState::Blocking);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AEODCharacterBase::DisableCharacterGuard()
{
	SetCharacterState(ECharacterState::IdleWalkRun);
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

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

void AEODCharacterBase::StartNormalAttack()
{
}

void AEODCharacterBase::StopNormalAttack()
{
}

void AEODCharacterBase::UpdateNormalAttackState(float DeltaTime)
{
}

void AEODCharacterBase::PlayToggleSheatheAnimation()
{
}

void AEODCharacterBase::UpdateDesiredYawFromAxisInput()
{
	DesiredRotationYawFromAxisInput = GetRotationYawFromAxisInput();
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		const float AngleTolerance = 1e-3f;
		if (!FMath::IsNearlyEqual(MoveComp->GetDesiredCustomRotationYaw(), DesiredRotationYawFromAxisInput, AngleTolerance))
		{
			MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYawFromAxisInput);
		}
	}
}

void AEODCharacterBase::OnPressedForward()
{
}

void AEODCharacterBase::OnPressedBackward()
{
}

void AEODCharacterBase::OnReleasedForward()
{
}

void AEODCharacterBase::OnReleasedBackward()
{
}


void AEODCharacterBase::UpdateMovementState(float DeltaTime)
{
	if (ForwardAxisValue < 0)
	{
		float Speed = IsValid(GetCharacterStatsComponent()) ? (DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier()) * 5 / 16 : DefaultWalkSpeed * 5 / 16;

		// float Speed = (DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = IsValid(GetCharacterStatsComponent()) ? DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier() : DefaultWalkSpeed;
		// float Speed = DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier();
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}

	//~ @old_code
	/*
	float DesiredRotationYaw = GetRotationYawFromAxisInput();
	if (!FMath::IsNearlyEqual(DesiredRotationYaw, GetActorRotation().Yaw, 1e-3f))
	{
		DeltaRotateCharacterToDesiredYaw(DesiredRotationYaw, DeltaTime);
	}
	*/
}

void AEODCharacterBase::UpdateGuardState(float DeltaTime)
{
	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0)
		{
			if (BlockMovementDirectionYaw != 90.f)
				SetBlockMovementDirectionYaw(90.f);
		}
		else if (RightAxisValue < 0)
		{
			if (BlockMovementDirectionYaw != -90.f)
				SetBlockMovementDirectionYaw(-90.f);
		}
		else
		{
			if (BlockMovementDirectionYaw != 0.f)
				SetBlockMovementDirectionYaw(0.f);
		}
	}
	else
	{
		float NewYaw = FMath::RadiansToDegrees(FMath::Atan2(RightAxisValue, ForwardAxisValue));
		if (BlockMovementDirectionYaw != NewYaw)
		{
			SetBlockMovementDirectionYaw(NewYaw);
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

void AEODCharacterBase::Server_SetWeaponSheathed_Implementation(bool bNewValue)
{
	SetWeaponSheathed(bNewValue);
}

bool AEODCharacterBase::Server_SetWeaponSheathed_Validate(bool bNewValue)
{
	return true;
}

void AEODCharacterBase::Server_SetGuardActive_Implementation(bool bValue)
{
	SetGuardActive(bValue);
}

bool AEODCharacterBase::Server_SetGuardActive_Validate(bool bValue)
{
	return true;
}

void AEODCharacterBase::Server_SetBlockMovementDirectionYaw_Implementation(float NewYaw)
{
	SetBlockMovementDirectionYaw(NewYaw);
}

bool AEODCharacterBase::Server_SetBlockMovementDirectionYaw_Validate(float NewYaw)
{
	return true;
}

void AEODCharacterBase::Multicast_SetCharacterRotation_Implementation(FRotator NewRotation)
{
	if (!IsLocallyControlled())
	{
		SetCharacterRotation(NewRotation);
	}
}
