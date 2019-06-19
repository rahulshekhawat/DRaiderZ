// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "GameSingleton.h"
#include "AIStatsComponent.h"
#include "EODAIControllerBase.h"
#include "EODWidgetComponent.h"
#include "AttackDodgedEvent.h"
#include "EODBlueprintFunctionLibrary.h"
#include "EODPlayerController.h"
#include "EODGameInstance.h"
#include "FloatingHealthBarWidget.h"
#include "AISkillsComponent.h"
#include "EODCharacterMovementComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include "GameFramework/CharacterMovementComponent.h"


const FName AAICharacterBase::AggroWidgetCompName(TEXT("Aggro Indicator"));
const FName AAICharacterBase::HealthWidgetCompName(TEXT("Health Indicator"));

AAICharacterBase::AAICharacterBase(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UAISkillsComponent>(AEODCharacterBase::GameplaySkillsComponentName))
{
	// Mob characters don't have strafe animations and so they must be rotated in the direction of their movement.
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	AggroWidgetComp = ObjectInitializer.CreateDefaultSubobject<UEODWidgetComponent>(this, AAICharacterBase::AggroWidgetCompName);
	if (AggroWidgetComp)
	{
		AggroWidgetComp->SetupAttachment(RootComponent);
	}

	HealthWidgetComp = ObjectInitializer.CreateDefaultSubobject<UEODWidgetComponent>(this, AAICharacterBase::HealthWidgetCompName);
	if (HealthWidgetComp)
	{
		HealthWidgetComp->SetupAttachment(RootComponent);
	}

	AggroWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AggroWidgetComp->SetCollisionProfileName(CollisionProfileNames::NoCollision);
	HealthWidgetComp->SetCollisionProfileName(CollisionProfileNames::NoCollision);
}

void AAICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetGameplaySkillsComponent())
	{
		GetGameplaySkillsComponent()->InitializeSkills(this);
	}
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetInCombat(false);
	UpdateHealthWidget();
}

void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller && Controller->IsLocalController())
	{
		bool bCanGuardAgainstAttacks = CanGuardAgainstAttacks();
		// If character wants to guard but it's guard is not yet active
		if (bWantsToGuard && !IsBlocking() && bCanGuardAgainstAttacks)
		{
			StartBlockingAttacks();
		}
		// If the character guard is active but it doesn't want to guard anymore
		else if (!bWantsToGuard && IsBlocking())
		{
			StopBlockingAttacks();
		}

		UpdateMovement(DeltaTime);
		UpdateRotation(DeltaTime);
	}
}

void AAICharacterBase::Destroyed()
{
	Super::Destroyed();
}

bool AAICharacterBase::CCEFlinch(const float BCAngle)
{
	if (CanFlinch())
	{
		if (FlinchMontage)
		{
			if (BCAngle <= 90)
			{
				PlayAnimMontage(FlinchMontage, 1.f, UCharacterLibrary::SectionName_ForwardFlinch);
			}
			else
			{
				PlayAnimMontage(FlinchMontage, 1.f, UCharacterLibrary::SectionName_BackwardFlinch);
			}
			return true;
		}
	}
	return false;
}

bool AAICharacterBase::CCEInterrupt(const float BCAngle)
{
	if (CanInterrupt())
	{
		if (InterruptMontage)
		{
			PreCCEStateEnter();

			float InterruptDuration = 0.f;
			int32 NumOfSections = 2; // Forward interrupt and backward interrupt
			if (BCAngle <= 90)
			{
				InterruptDuration = (PlayAnimMontage(InterruptMontage, 1.f, UCharacterLibrary::SectionName_ForwardInterrupt)) / 2;
			}
			else
			{
				InterruptDuration = (PlayAnimMontage(InterruptMontage, 1.f, UCharacterLibrary::SectionName_BackwardInterrupt)) / 2;
			}

			CharacterStateInfo.CharacterState = ECharacterState::GotHit;
			bCharacterStateAllowsMovement = false;
			bCharacterStateAllowsRotation = false;
			UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
			if (MoveComp)
			{
				MoveComp->bUseControllerDesiredRotation = false;
			}

			InterruptDuration = InterruptDuration - InterruptMontage->BlendOut.GetBlendTime();
			if (InterruptDuration > 0.f)
			{
				UWorld* World = GetWorld();
				check(World);
				World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::ResetState, InterruptDuration);
			}
			else
			{
				ResetState();
			}

			return true;
		}
	}
	return false;
}

bool AAICharacterBase::CCEStun(const float Duration)
{
	if (CanStun())
	{
		if (StunMontage)
		{
			PreCCEStateEnter();

			PlayAnimMontage(StunMontage, 1.f);

			UWorld* World = GetWorld();
			check(World);
			World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::CCERemoveStun, Duration, false);

			CharacterStateInfo.CharacterState = ECharacterState::GotHit;
			bCharacterStateAllowsMovement = false;
			bCharacterStateAllowsRotation = false;
			UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
			if (MoveComp)
			{
				MoveComp->bUseControllerDesiredRotation = false;
			}

			return true;
		}
	}
	return false;
}

void AAICharacterBase::CCERemoveStun()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance && StunMontage && AnimInstance->Montage_IsPlaying(StunMontage))
	{
		AnimInstance->Montage_Stop(StunMontage->BlendOut.GetBlendTime(), StunMontage);
		ResetState();
	}

	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
}

bool AAICharacterBase::CCEFreeze(const float Duration)
{
	if (CanFreeze() && GetMesh())
	{
		PreCCEStateEnter();

		GetMesh()->GlobalAnimRateScale = 0.f;

		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::CCEUnfreeze, Duration, false);

		CharacterStateInfo.CharacterState = ECharacterState::GotHit;
		bCharacterStateAllowsMovement = false;
		bCharacterStateAllowsRotation = false;
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->bUseControllerDesiredRotation = false;
		}

		return true;
	}
	return false;
}

void AAICharacterBase::CCEUnfreeze()
{
	if (GetMesh())
	{
		GetMesh()->GlobalAnimRateScale = 1.f;
	}

	ResetState();

	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
}

bool AAICharacterBase::CCEKnockdown(const float Duration)
{
	if (CanKnockdown())
	{
		if (KnockdownMontage)
		{
			PreCCEStateEnter();

			PlayAnimMontage(KnockdownMontage, 1.f, UCharacterLibrary::SectionName_KnockdownStart);

			UWorld* World = GetWorld();
			check(World);
			World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::CCEEndKnockdown, Duration, false);

			CharacterStateInfo.CharacterState = ECharacterState::GotHit;
			bCharacterStateAllowsMovement = false;
			bCharacterStateAllowsRotation = false;
			UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
			if (MoveComp)
			{
				MoveComp->bUseControllerDesiredRotation = false;
			}

			return true;
		}
	}
	return false;
}

void AAICharacterBase::CCEEndKnockdown()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance && KnockdownMontage && AnimInstance->Montage_IsPlaying(KnockdownMontage))
	{
		PlayAnimMontage(KnockdownMontage, 1.f, UCharacterLibrary::SectionName_KnockdownEnd);
		ResetState();
	}

	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
}

bool AAICharacterBase::CCEKnockback(const float Duration, const FVector& ImpulseDirection)
{
	bool bKnockdownInitiated = CCEKnockdown(Duration);
	if (bKnockdownInitiated)
	{
		PushBack(ImpulseDirection);
		return true;
	}
	return false;
}

bool AAICharacterBase::IsEnemyOf(ICombatInterface* TargetCI) const
{
	AActor* Target = TargetCI->GetInterfaceOwner();
	if (Target->IsA(AAICharacterBase::StaticClass()))
	{
		return false;
	}
	return true;
}

USoundBase* AAICharacterBase::GetMeleeHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	USoundBase* Sound = bCritHit ? CritHitSound : HitSound;
	return Sound ? Sound : HitSound; // A check just in case CritHitSound is not set
}

USoundBase* AAICharacterBase::GetMeleeHitMissSound() const
{
	return HitMissSound;
}

void AAICharacterBase::SetInCombat(bool bValue)
{
	bInCombat = bValue;
	SetIsRunning(bInCombat);

	if (bInCombat)
	{
		UUserWidget* AggroWidget = AggroWidgetComp->GetUserWidgetObject();
		if (AggroWidget)
		{
			AggroWidget->SetVisibility(ESlateVisibility::Visible);
		}

		UUserWidget* HealthWidget = HealthWidgetComp->GetUserWidgetObject();
		if (HealthWidget)
		{
			HealthWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		UUserWidget* AggroWidget = AggroWidgetComp->GetUserWidgetObject();
		if (AggroWidget)
		{
			AggroWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		UUserWidget* HealthWidget = HealthWidgetComp->GetUserWidgetObject();
		if (HealthWidget)
		{
			HealthWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AAICharacterBase::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	// if (AnimMontage == FlinchAnimMontage || bUsingUniqueSkill)
	if (AnimMontage == FlinchMontage)
	{
		return;
	}

	//~ @todo
	/*
	if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimMontage.Get() == AnimMontage)
	{
		GetLastUsedSkill().LastUsedSkillID = GetCurrentActiveSkillID();
		GetLastUsedSkill().bInterrupted = bInterrupted;

		SetCurrentActiveSkillID(NAME_None);
		SetCurrentActiveSkill(nullptr);
	}
	*/

	//~ @todo
	/*
	if (!bInterrupted && GetCharacterState() != ECharacterState::Dead)
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}
	*/
}

void AAICharacterBase::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	// @todo
}

bool AAICharacterBase::CanUseAnySkill() const
{
	return false;
}

bool AAICharacterBase::CanUseSkill(FName SkillID, UGameplaySkillBase* Skill)
{
	bool bCanUseSkill = false;
	if (GetGameplaySkillsComponent())
	{
		uint8 SkillIndex = GetGameplaySkillsComponent()->GetSkillIndexForSkillGroup(SkillID);
		bCanUseSkill = GetGameplaySkillsComponent()->CanUseSkill(SkillIndex, Skill);
	}
	return bCanUseSkill;
}

bool AAICharacterBase::UseSkill_Implementation(FName SkillID, UGameplaySkillBase* Skill)
{
	if (CanUseSkill(SkillID, Skill))
	{
		uint8 SkillIndex = GetGameplaySkillsComponent()->GetSkillIndexForSkillGroup(SkillID);
		GetGameplaySkillsComponent()->TriggerSkill(SkillIndex, Skill);
		return true;
	}

	return false;
}

EEODTaskStatus AAICharacterBase::CheckSkillStatus(FName SkillID)
{
	if (GetCurrentActiveSkillID() == SkillID)
	{
		return EEODTaskStatus::Active;
	}
	
	if (GetLastUsedSkill().LastUsedSkillID != SkillID)
	{
		return EEODTaskStatus::Inactive;
	}

	if (GetLastUsedSkill().bInterrupted)
	{
		return EEODTaskStatus::Aborted;
	}
	else
	{
		return EEODTaskStatus::Finished;
	}
}

FName AAICharacterBase::GetMostWeightedSkillID(const AEODCharacterBase* TargetCharacter) const
{
	FName MostWeightedSkillID = NAME_None;
	UAISkillsComponent* SkillsComp = Cast<UAISkillsComponent>(GetGameplaySkillsComponent());
	if (SkillsComp)
	{
		MostWeightedSkillID = SkillsComp->GetMostWeightedSkillID(TargetCharacter);
	}
	return MostWeightedSkillID;
}

FName AAICharacterBase::GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const
{
	FName MostWeightedSkillID = NAME_None;
	UAISkillsComponent* SkillsComp = Cast<UAISkillsComponent>(GetGameplaySkillsComponent());
	if (SkillsComp)
	{
		MostWeightedSkillID = SkillsComp->GetMostWeightedMeleeSkillID(TargetCharacter);
	}
	return MostWeightedSkillID;
}

FName AAICharacterBase::GetMostWeightedRangedSkillID(const AEODCharacterBase* TargetCharacter) const
{
	FName MostWeightedSkillID = NAME_None;
	UAISkillsComponent* SkillsComp = Cast<UAISkillsComponent>(GetGameplaySkillsComponent());
	if (SkillsComp)
	{
		MostWeightedSkillID = SkillsComp->GetMostWeightedRangedSkillID(TargetCharacter);
	}
	return MostWeightedSkillID;
}

void AAICharacterBase::OnSkillActivated(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	SetLastUsedSkill(FLastUsedSkillInfo());
}

bool AAICharacterBase::CanAssistAlly_Implementation()
{
	return false;
}

void AAICharacterBase::AssistanceRequested_Implementation(const AAICharacterBase* Requestor)
{
}

void AAICharacterBase::UpdateMovement(float DeltaTime)
{
	if (!bCharacterStateAllowsMovement)
	{
		return;
	}

	float NewSpeed = 0.f;
	if (IsBlocking())
	{
		NewSpeed = DefaultWalkSpeedWhileBlocking * MovementSpeedModifier;
	}
	else
	{
		float DesiredSpeed = IsRunning() ? DefaultRunSpeed : DefaultWalkSpeed;
		NewSpeed = DesiredSpeed * MovementSpeedModifier;
	}

	SetWalkSpeed(NewSpeed);

	float CurrentSpeed = GetVelocity().Size();
	if (CurrentSpeed > 0.f)
	{
		SetCharacterMovementDirection(ECharMovementDirection::F);
	}
}

void AAICharacterBase::UpdateRotation(float DeltaTime)
{
}

void AAICharacterBase::InitiateDeathSequence_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	float DeathMontageDuration = 0.f;
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);

		if (DieMontage)
		{
			DeathMontageDuration = AnimInstance->Montage_Play(DieMontage);
		}
	}

	AEODAIControllerBase* AIC = Cast<AEODAIControllerBase>(Controller);
	UBrainComponent* BrainComp = AIC ? AIC->GetBrainComponent() : nullptr;

	if (BrainComp)
	{
		BrainComp->StopLogic(FString("Character Died"));
	}

	UCapsuleComponent* CapComp = GetCapsuleComponent();
	if (CapComp)
	{
		CapComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (DeathMontageDuration > 1.f)
	{
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(WidgetTimerHandle, this, &AAICharacterBase::DestroyFloatingHealthWidget, 1.f, false);
	}
	else
	{
		DestroyFloatingHealthWidget();
	}

	CharacterStateInfo.CharacterState = ECharacterState::Dead;
}

void AAICharacterBase::DestroyFloatingHealthWidget()
{
	if (HealthWidgetComp)
	{
		UUserWidget* Widget = HealthWidgetComp->GetUserWidgetObject();
		if (Widget)
		{
			Widget->RemoveFromParent();
		}

		HealthWidgetComp->SetWidget(nullptr);

		HealthWidgetComp->DestroyComponent();
		HealthWidgetComp = nullptr;
	}

	if (AggroWidgetComp)
	{
		UUserWidget* Widget = AggroWidgetComp->GetUserWidgetObject();
		if (Widget)
		{
			Widget->RemoveFromParent();
		}

		AggroWidgetComp->SetWidget(nullptr);

		AggroWidgetComp->DestroyComponent();
		AggroWidgetComp = nullptr;
	}
}

void AAICharacterBase::UpdateHealthWidget()
{
	if (HealthWidgetComp)
	{
		UFloatingHealthBarWidget* HealthWidget = Cast<UFloatingHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(Health.MaxValue, Health.CurrentValue);
		}
	}
}

void AAICharacterBase::UpdateHealth(int32 MaxHealth, int32 CurrentHealth)
{
	Super::UpdateHealth(MaxHealth, CurrentHealth);
	UpdateHealthWidget();
}

void AAICharacterBase::OnRep_InCombat()
{
	SetInCombat(IsInCombat());
}

void AAICharacterBase::OnRep_Health(FCharacterStat& OldHealth)
{
	UpdateHealthWidget();

	if (Health.CurrentValue <= 0)
	{
		InitiateDeathSequence();
	}
}
