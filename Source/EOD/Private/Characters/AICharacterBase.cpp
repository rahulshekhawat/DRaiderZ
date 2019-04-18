// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "GameSingleton.h"
#include "EODPreprocessors.h"
#include "AIStatsComponent.h"
#include "EODAIControllerBase.h"
#include "EODWidgetComponent.h"
#include "AttackDodgedEvent.h"
#include "EODBlueprintFunctionLibrary.h"
#include "EODPlayerController.h"
#include "EODGameInstance.h"
#include "FloatingHealthBarWidget.h"
#include "AISkillsComponent.h"

#include "Engine/Engine.h"
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
}

TSharedPtr<FAttackResponse> AAICharacterBase::ReceiveAttack(AActor* HitInstigator, ICombatInterface* InstigatorCI, const TSharedPtr<FAttackInfo>& AttackInfoPtr, const FHitResult& DirectHitResult, const bool bLineHitResultFound, const FHitResult& LineHitResult)
{
	TSharedPtr<FAttackResponse> AttackResponsePtr = TSharedPtr<FAttackResponse>(nullptr);

	AEODAIControllerBase* AIController = Cast<AEODAIControllerBase>(Controller);
	UStatsComponentBase* StatsComp = AIController ? AIController->GetStatsComponent() : nullptr;

	if (!AttackInfoPtr.IsValid() || !InstigatorCI || !StatsComp)
	{
		return AttackResponsePtr;
	}

	AEODCharacterBase* InstigatingChar = Cast<AEODCharacterBase>(HitInstigator);
	bool bIsInstigatorAPlayer = InstigatingChar && InstigatingChar->Controller && InstigatingChar->Controller->IsLocalPlayerController();
	if (!AttackInfoPtr->bUndodgable && this->IsDodgingDamage())
	{
		if (bIsInstigatorAPlayer)
		{

			// DisplayTextOnPlayerScreen(FString("Dodge"), )
		}

		UAttackDodgedEvent* DodgeEvent = NewObject<UAttackDodgedEvent>();
		DodgeEvent->AddToRoot();
		this->OnDodgingAttack.Broadcast(this, HitInstigator, this, DodgeEvent);
		DodgeEvent->RemoveFromRoot();
		DodgeEvent->MarkPendingKill();

		AttackResponsePtr = MakeShareable(new FAttackResponse);
		if (AttackResponsePtr.IsValid())
		{
			AttackResponsePtr->ActualDamage = 0.f;
			AttackResponsePtr->bCritHit = false;
			AttackResponsePtr->CrowdControlEffect = ECrowdControlEffect::Flinch;
			AttackResponsePtr->DamageResult = EDamageResult::Dodged;
		}

		// Replicate Hit Info
		FReceivedHitInfo ReceivedHitInfo;
		ReceivedHitInfo.DamageResult = EDamageResult::Dodged;
		ReceivedHitInfo.HitInstigator = HitInstigator;
		ReceivedHitInfo.ReplicationIndex = LastReceivedHit.ReplicationIndex + 1;
		LastReceivedHit = ReceivedHitInfo;

		return AttackResponsePtr;
	}

	bool bCritHit = AttackInfoPtr->CritRate	>= FMath::RandRange(0.f, 100.f) ? true : false;
	bool bAttackBlocked = false;
	float BCAngle = 0.f;
	if (bLineHitResultFound)
	{
		BCAngle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(GetActorForwardVector(), LineHitResult.ImpactNormal);
		if (!AttackInfoPtr->bUnblockable && this->IsBlockingDamage())
		{
			bAttackBlocked = BCAngle < UCombatLibrary::BlockDetectionAngle ? true : false;
			if (bAttackBlocked)
			{
				//~ @todo block event
				PlayAttackBlockedAnimation();
			}
		}
	}
	else
	{
		BCAngle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(GetActorForwardVector(), DirectHitResult.ImpactNormal);
	}

	float ActualDamage = GetActualDamage(HitInstigator, InstigatorCI, AttackInfoPtr, bCritHit, bAttackBlocked);
	StatsComp->ModifyCurrentHealth(-ActualDamage);
	int32 CurrentHP = StatsComp->GetCurrentHealth();

	bool bCCEApplied = false;
	if (!bAttackBlocked)
	{
		switch (AttackInfoPtr->CrowdControlEffect)
		{
		case ECrowdControlEffect::Flinch:
			bCCEApplied = CCEFlinch(BCAngle);
			break;
		case ECrowdControlEffect::Interrupt:
			bCCEApplied = CCEInterrupt(BCAngle);
			break;
		case ECrowdControlEffect::KnockedDown:
			bCCEApplied = CCEKnockdown(AttackInfoPtr->CrowdControlEffectDuration);
			break;
		case ECrowdControlEffect::KnockedBack:
			bCCEApplied = CCEKnockback(AttackInfoPtr->CrowdControlEffectDuration, HitInstigator->GetActorForwardVector());
			break;
		case ECrowdControlEffect::Stunned:
			bCCEApplied = CCEStun(AttackInfoPtr->CrowdControlEffectDuration);
			break;
		case ECrowdControlEffect::Crystalized:
			bCCEApplied = CCEFreeze(AttackInfoPtr->CrowdControlEffectDuration);
			break;
		default:
			break;
		}
	}

	AttackResponsePtr = MakeShareable(new FAttackResponse);
	AttackResponsePtr->ActualDamage = ActualDamage;
	AttackResponsePtr->CrowdControlEffect = bCCEApplied ? AttackInfoPtr->CrowdControlEffect : ECrowdControlEffect::Flinch;
	if (bAttackBlocked)
	{
		AttackResponsePtr->DamageResult = ActualDamage > 0 ? EDamageResult::Blocked : EDamageResult::Nullified;
	}
	else
	{
		AttackResponsePtr->DamageResult = ActualDamage > 0 ? EDamageResult::Damaged : EDamageResult::Nullified;
	}


	if (bCCEApplied)
	{
		AttackResponsePtr->CrowdControlEffect = AttackInfoPtr->CrowdControlEffect;
	}

	if (!bAttackBlocked)
	{
		SetOffTargetSwitch(TargetSwitchDuration);
	}

	// Replicate Hit Info
	FReceivedHitInfo ReceivedHitInfo;
	ReceivedHitInfo.ActualDamage = AttackResponsePtr->ActualDamage;
	ReceivedHitInfo.DamageResult = AttackResponsePtr->DamageResult;
	ReceivedHitInfo.CrowdControlEffect = AttackResponsePtr->CrowdControlEffect;
	ReceivedHitInfo.CrowdControlEffectDuration = AttackInfoPtr->CrowdControlEffectDuration;
	ReceivedHitInfo.BCAngle = BCAngle;
	ReceivedHitInfo.bCritHit = AttackResponsePtr->bCritHit;
	ReceivedHitInfo.HitInstigator = HitInstigator;
	if (bLineHitResultFound)
	{
		ReceivedHitInfo.HitLocation = LineHitResult.ImpactPoint;
	}
	else
	{
		ReceivedHitInfo.HitLocation = DirectHitResult.ImpactPoint;
	}
	ReceivedHitInfo.ReplicationIndex = LastReceivedHit.ReplicationIndex + 1;
	LastReceivedHit = ReceivedHitInfo;


	UEODGameInstance* EODGI = Cast<UEODGameInstance>(GetGameInstance());
	if (EODGI)
	{
		EODGI->DisplayDamageNumbers(LastReceivedHit.ActualDamage, LastReceivedHit.bCritHit, this, LastReceivedHit.HitInstigator, LastReceivedHit.HitLocation);
	}

	return AttackResponsePtr;
}

float AAICharacterBase::GetActualDamage(AActor* HitInstigator, ICombatInterface* InstigatorCI, const TSharedPtr<FAttackInfo>& AttackInfoPtr, const bool bCritHit, const bool bAttackBlocked)
{
	float ActualDamage = 0.f;

	if (!AttackInfoPtr.IsValid())
	{
		return ActualDamage;
	}

	if (bCritHit)
	{
		ActualDamage = AttackInfoPtr->CritDamage;
	}
	else
	{
		ActualDamage = AttackInfoPtr->NormalDamage;
	}

	if (bAttackBlocked)
	{
		AEODAIControllerBase* AIController = Cast<AEODAIControllerBase>(Controller);
		UStatsComponentBase* StatsComp = AIController ? AIController->GetStatsComponent() : nullptr;

		float DamageReductionOnBlock = 0.1f;
		if (StatsComp)
		{
			if (AttackInfoPtr->DamageType == EDamageType::Magickal)
			{
				DamageReductionOnBlock = StatsComp->GetMagickDamageReductionOnBlock();
			}
			else
			{
				DamageReductionOnBlock = StatsComp->GetPhysicalDamageReductionOnBlock();
			}
		}

		ActualDamage = ActualDamage * (1 - DamageReductionOnBlock);
	}

	return ActualDamage;
}

bool AAICharacterBase::CCEFlinch_Implementation(const float BCAngle)
{
	if (CanFlinch() && FlinchMontage)
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

	return false;
}

bool AAICharacterBase::CCEInterrupt_Implementation(const float BCAngle)
{
	if (CanInterrupt() && InterruptMontage)
	{
		if (BCAngle <= 90)
		{
			PlayAnimMontage(InterruptMontage, 1.f, UCharacterLibrary::SectionName_ForwardInterrupt);
		}
		else
		{
			PlayAnimMontage(InterruptMontage, 1.f, UCharacterLibrary::SectionName_BackwardInterrupt);
		}

		return true;
	}

	return false;
}

bool AAICharacterBase::CCEStun_Implementation(const float Duration)
{
	if (CanStun())
	{
		PlayStunAnimation();

		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCERemoveStun, Duration, false);
		return true;
	}

	return false;
}

void AAICharacterBase::CCERemoveStun_Implementation()
{
	StopStunAnimation();

	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
	// @todo Restore character state to IdleWalkRun if necessary (if OnMontageBlendingOut event doesn't restore character state to IdleWalkRun)
}

bool AAICharacterBase::CCEFreeze_Implementation(const float Duration)
{
	if (CanFreeze() && GetMesh())
	{
		GetMesh()->GlobalAnimRateScale = 0.f;

		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::CCEUnfreeze, Duration, false);
		return true;
	}

	return false;
}

void AAICharacterBase::CCEUnfreeze_Implementation()
{
	if (GetMesh())
	{
		GetMesh()->GlobalAnimRateScale = 1.f;
	}

	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
}

bool AAICharacterBase::CCEKnockdown_Implementation(const float Duration)
{
	if (CanKnockdown() && KnockdownMontage)
	{
		PlayAnimMontage(KnockdownMontage, 1.f, UCharacterLibrary::SectionName_KnockdownStart);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);

		return true;
	}

	return false;
}

void AAICharacterBase::CCEEndKnockdown_Implementation()
{
	PlayAnimMontage(KnockdownMontage, 1.f, UCharacterLibrary::SectionName_KnockdownEnd);
}

bool AAICharacterBase::CCEKnockback_Implementation(const float Duration, const FVector & ImpulseDirection)
{
	if (CanKnockdown() && KnockdownMontage)
	{
		PlayAnimMontage(KnockdownMontage, 1.f, UCharacterLibrary::SectionName_KnockdownStart);

		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);
		PushBack(ImpulseDirection);
		return true;
	}

	return false;
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

bool AAICharacterBase::UseSkill_Implementation(FName SkillID, UGameplaySkillBase* Skill)
{
	if (!CanUseSkill(SkillID, Skill))
	{
		return false;
	}

	if (GetGameplaySkillsComponent())
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

bool AAICharacterBase::CanAssistAlly_Implementation()
{
	return false;
}

void AAICharacterBase::AssistanceRequested_Implementation(const AAICharacterBase * Requestor)
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

void AAICharacterBase::UpdateHealthWidget()
{
	if (HealthWidgetComp)
	{
		UFloatingHealthBarWidget* HealthWidget = Cast<UFloatingHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(Health.BaseValue, Health.MaxValue, Health.CurrentValue);
		}
	}
}

void AAICharacterBase::OnHealthUpdated(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth)
{
	Health.SetStatValues(BaseHealth, MaxHealth, CurrentHealth);
	UpdateHealthWidget();
}

void AAICharacterBase::OnRep_Health(FCharacterStat& OldHealth)
{
	UpdateHealthWidget();
}

void AAICharacterBase::OnRep_LastReceivedHit(const FReceivedHitInfo& OldHitInfo)
{
	if (LastReceivedHit.DamageResult == EDamageResult::Dodged)
	{
		//~ @todo Display dodge message
		//~ @todo Play dodge sound
	}
	else if (LastReceivedHit.DamageResult == EDamageResult::Blocked)
	{
		//~ @todo Play attack blocked animation
		//~ @todo Play attack blocked sound
	}
	else if (LastReceivedHit.DamageResult == EDamageResult::Immune)
	{
		//~ @todo Display immune message
	}
	else if (LastReceivedHit.DamageResult == EDamageResult::Nullified)
	{
		SetOffTargetSwitch(TargetSwitchDuration);
		
	}
	else if (LastReceivedHit.DamageResult == EDamageResult::Damaged)
	{
		SetOffTargetSwitch(TargetSwitchDuration);
	}

	UEODGameInstance* EODGI = Cast<UEODGameInstance>(GetGameInstance());
	if (EODGI)
	{
		EODGI->DisplayDamageNumbers(LastReceivedHit.ActualDamage, LastReceivedHit.bCritHit, this, LastReceivedHit.HitInstigator, LastReceivedHit.HitLocation);
	}
	
	switch (LastReceivedHit.CrowdControlEffect)
	{
	case ECrowdControlEffect::Flinch:
		CCEFlinch(LastReceivedHit.BCAngle);
		break;
	case ECrowdControlEffect::Interrupt:
		CCEInterrupt(LastReceivedHit.BCAngle);
		break;
	case ECrowdControlEffect::KnockedDown:
		CCEKnockdown(LastReceivedHit.CrowdControlEffectDuration);
		break;
	case ECrowdControlEffect::KnockedBack:
		if (LastReceivedHit.HitInstigator)
		{
			CCEKnockback(LastReceivedHit.CrowdControlEffectDuration, LastReceivedHit.HitInstigator->GetActorForwardVector());
		}
		break;
	case ECrowdControlEffect::Stunned:
		CCEStun(LastReceivedHit.CrowdControlEffectDuration);
		break;
	case ECrowdControlEffect::Crystalized:
		CCEFreeze(LastReceivedHit.CrowdControlEffectDuration);
		break;
	default:
		break;
	}
}
