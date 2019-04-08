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
#include "GenericPlatform/GenericPlatformTime.h"
#include "EODGameInstance.h"

#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacterBase::AAICharacterBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UAIStatsComponent>(FName("Character Stats Component")))
{
	// Mob characters don't have strafe animations and so they must be rotated in the direction of their movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AggroWidgetComp = ObjectInitializer.CreateDefaultSubobject<UEODWidgetComponent>(this, TEXT("Aggro Indicator"));
	AggroWidgetComp->SetupAttachment(RootComponent);
	// AggroWidgetComp->SetupAttachment(GetMesh(), FName("dummy_ef_head"));
	HealthWidgetComp = ObjectInitializer.CreateDefaultSubobject<UEODWidgetComponent>(this, TEXT("Health Indicator"));
	HealthWidgetComp->SetupAttachment(RootComponent);
	// HealthWidgetComp->SetupAttachment(GetMesh(), FName("dummy_ef_head"));

	AggroWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AggroWidgetComp->SetCollisionProfileName(FName("NoCollision"));
	HealthWidgetComp->SetCollisionProfileName(FName("NoCollision"));
}

void AAICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeSkills();
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetInCombat(false);

	// Following line is used to update the floating health bar widget (floating health bar widget is NULL when CurrentHealth is initially initialized)
	// GetCharacterStatsComponent()->ModifyCurrentHealth(0);
}

void AAICharacterBase::Destroyed()
{
	if (SkillAnimationsStreamableHandle.IsValid())
	{
		SkillAnimationsStreamableHandle.Get()->ReleaseHandle();
		SkillAnimationsStreamableHandle.Reset();
	}
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


	/*

	/*
	if (ResultingHitCharacterHP <= 0)
	{
		HitCharacter->InitiateDeathSequence();
	}
	NativeDisplayDamage(HitInstigator, HitCharacter, LineHitResult, ActualDamage, bCritHit);

	// @todo make camera shake interesting
	PlayCameraShake(ECameraShakeType::Medium, LineHitResult.ImpactPoint);
	SpawnHitSFX(LineHitResult.ImpactPoint, LineHitResult.ImpactNormal);
	PlayHitSound(HitInstigator, LineHitResult.ImpactPoint, bCritHit);

	if (bAttackBlocked)
	{
		return;
	}

	HitCharacter->SetOffTargetSwitch();
	*/




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

UEODWidgetComponent* AAICharacterBase::BP_GetAggroWidgetComp() const
{
	return GetAggroWidgetComp();
}

UEODWidgetComponent* AAICharacterBase::BP_GetHealthWidgetComp() const
{
	return GetHealthWidgetComp();
}

bool AAICharacterBase::CCEFlinch_Implementation(const float BCAngle)
{
	if (CanFlinch() && FlinchAnimMontage)
	{
		if (BCAngle <= 90)
		{
			PlayAnimMontage(FlinchAnimMontage, 1.f, UCharacterLibrary::SectionName_ForwardFlinch);
		}
		else
		{
			PlayAnimMontage(FlinchAnimMontage, 1.f, UCharacterLibrary::SectionName_BackwardFlinch);
		}

		return true;
	}

	return false;
}

bool AAICharacterBase::CCEInterrupt_Implementation(const float BCAngle)
{
	if (CanInterrupt() && HitEffectsAnimMontage)
	{
		if (BCAngle <= 90)
		{
			// NetPlayAnimMontage(HitEffectsAnimMontage, UCharacterLibrary::SectionName_ForwardInterrupt);
			PlayAnimMontage(HitEffectsAnimMontage, 1.f, UCharacterLibrary::SectionName_ForwardInterrupt);
		}
		else
		{
			// NetPlayAnimMontage(HitEffectsAnimMontage, UCharacterLibrary::SectionName_BackwardInterrupt);
			PlayAnimMontage(HitEffectsAnimMontage, 1.f, UCharacterLibrary::SectionName_BackwardInterrupt);
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
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCERemoveStun, Duration, false);

		return true;
	}

	return false;
}

void AAICharacterBase::CCERemoveStun_Implementation()
{
	StopStunAnimation();
	// @todo Restore character state to IdleWalkRun if necessary (if OnMontageBlendingOut event doesn't restore character state to IdleWalkRun)
}

bool AAICharacterBase::CCEFreeze_Implementation(const float Duration)
{
	// @todo maybe just freeze animation instead of freezing entire character since it might freeze additional effects like glow

	if (CanFreeze())
	{
		CustomTimeDilation = 0;
		//~ @todo
		// GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::EndFreeze, Duration, false);

		return true;
	}

	return false;
}

void AAICharacterBase::CCEUnfreeze_Implementation()
{
	//~ @todo
	// CustomTimeDilation = GetCharacterStatsComponent()->GetActiveTimeDilation();
}

bool AAICharacterBase::CCEKnockdown_Implementation(const float Duration)
{
	if (CanKnockdown() && HitEffectsAnimMontage)
	{
		// NetPlayAnimMontage(HitEffectsAnimMontage, UCharacterLibrary::SectionName_KnockdownStart);
		PlayAnimMontage(HitEffectsAnimMontage, 1.f, UCharacterLibrary::SectionName_KnockdownStart);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);

		return true;
	}

	return false;
}

void AAICharacterBase::CCEEndKnockdown_Implementation()
{
	// NetPlayAnimMontage(HitEffectsAnimMontage, UCharacterLibrary::SectionName_KnockdownEnd);
	PlayAnimMontage(HitEffectsAnimMontage, 1.f, UCharacterLibrary::SectionName_KnockdownEnd);
}

bool AAICharacterBase::CCEKnockback_Implementation(const float Duration, const FVector & ImpulseDirection)
{
	if (CanKnockdown() && HitEffectsAnimMontage)
	{
		//~ @todo
		// PlayAnimationMontage(HitEffectsAnimMontage,
			// UCharacterLibrary::SectionName_KnockdownStart,
			// ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);
		PushBack(ImpulseDirection);
		return true;
	}

	return false;
}

void AAICharacterBase::SetInCombat(bool bValue)
{
	if (bValue)
	{
		if (AggroWidgetComp->GetUserWidgetObject())
		{
			AggroWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		}
		if (HealthWidgetComp->GetUserWidgetObject())
		{
			HealthWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (AggroWidgetComp->GetUserWidgetObject())
		{
			AggroWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
		if (HealthWidgetComp->GetUserWidgetObject())
		{
			HealthWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	bInCombat = bValue;
	UpdateMaxWalkSpeed();
}

void AAICharacterBase::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	// if (AnimMontage == FlinchAnimMontage || bUsingUniqueSkill)
	if (AnimMontage == FlinchAnimMontage)
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
	if (CanUseAnySkill())
	{
		//~ @todo
		// FSkillTableRow* SkillToUse = GetSkill(SkillID, FString("AAICharacterBase::UseSkill, looking up AI skills for use"));
		FSkillTableRow* SkillToUse = nullptr;

		if (!SkillToUse)
		{
#if MESSAGE_LOGGING_ENABLED
			FString Message = FString("Couldn't find AI character skill : ") + SkillID.ToString();
			UKismetSystemLibrary::PrintString(this, FString());
#endif // MESSAGE_LOGGING_ENABLED
			return false;
		}

		if (SkillToUse->AnimMontage.Get())
		{
			//~ @todo
			// PlayAnimationMontage(SkillToUse->AnimMontage.Get(), SkillToUse->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		}
		SetCurrentActiveSkillID(SkillID);
		// SetCurrentActiveSkill(SkillToUse);
		//~ @todo
		// SkillIDToWeightMap[SkillID] = SkillIDToWeightMap[SkillID] - 1;
		// bSkillAllowsMovement = SkillToUse->bAllowsMovement;
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
	TArray<FName> EligibleSkills;
	// TArray<FName> MostWeightedSkills;

	if (TargetCharacter->HasBeenHit())
	{
		for (FName SkillID : MeleeSkills)
		{
			if (FlinchSkills.Contains(SkillID))
			{
				EligibleSkills.Add(SkillID);
			}
		}

		for (FName SkillID : EligibleSkills)
		{
			if (MostWeightedSkillID == NAME_None)
			{
				MostWeightedSkillID = SkillID;
				continue;
			}

			/* @fix
			if (SkillIDToWeightMap[SkillID] > SkillIDToWeightMap[MostWeightedSkillID])
			{
				MostWeightedSkillID = SkillID;
			}
			*/
		}
	}
	else
	{
		for (FName SkillID : MeleeSkills)
		{
			if (!FlinchSkills.Contains(SkillID))
			{
				EligibleSkills.Add(SkillID);
			}
		}

		for (FName SkillID : EligibleSkills)
		{
			if (MostWeightedSkillID == NAME_None)
			{
				MostWeightedSkillID = SkillID;
				continue;
			}

			/* @fix
			if (SkillIDToWeightMap[SkillID] > SkillIDToWeightMap[MostWeightedSkillID])
			{
				MostWeightedSkillID = SkillID;
			}
			*/
		}
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

void AAICharacterBase::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;	
}

void AAICharacterBase::InitializeSkills()
{
	//~ @todo
	/*
	if (!SkillsDataTable)
	{
		return;
	}

	TArray<FSoftObjectPath> AnimationsToLoad;
	TArray<FName> SkillIDs = SkillsDataTable->GetRowNames();
	for (const FName& SkillID : SkillIDs)
	{
		FSkillTableRow* Skill = GetSkill(SkillID, FString("AAICharacterBase::InitializeSkills(), looking for AI skill"));
		if (!Skill)
		{
#if MESSAGE_LOGGING_ENABLED
			FString Message = FString("Couldn't find skil : ") + SkillID.ToString();
			UKismetSystemLibrary::PrintString(this, Message);
#endif // MESSAGE_LOGGING_ENABLED
			return;
		}
		check(Skill); // Redundant
		if (Skill->SkillType == ESkillType::BuffParty)
		{
			PartyBuffSkills.Add(SkillID);
		}
		else if (Skill->SkillType == ESkillType::BuffSelf)
		{
			SelfBuffSkills.Add(SkillID);
		}
		else if (Skill->SkillType == ESkillType::DamageMelee)
		{
			MeleeSkills.Add(SkillID);
			if (Skill->CrowdControlEffect == ECrowdControlEffect::Crystalized)
			{
				CrystalizeSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::Flinch)
			{
				FlinchSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::Interrupt)
			{
				InterruptSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::KnockedBack)
			{
				KnockBackSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::KnockedDown)
			{
				KnockDownSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::Stunned)
			{
				StunSkills.Add(SkillID);
			}
		}
		else if (Skill->SkillType == ESkillType::DamageRanged)
		{
			RangedSkills.Add(SkillID);
			if (Skill->CrowdControlEffect == ECrowdControlEffect::Crystalized)
			{
				CrystalizeSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::Flinch)
			{
				FlinchSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::Interrupt)
			{
				InterruptSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::KnockedBack)
			{
				KnockBackSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::KnockedDown)
			{
				KnockDownSkills.Add(SkillID);
			}
			else if (Skill->CrowdControlEffect == ECrowdControlEffect::Stunned)
			{
				StunSkills.Add(SkillID);
			}
		}
		else if (Skill->SkillType == ESkillType::DebuffEnemy)
		{
			DebuffSkills.Add(SkillID);
		}
		else if (Skill->SkillType == ESkillType::HealParty)
		{
			PartyHealSkills.Add(SkillID);
		}
		else if (Skill->SkillType == ESkillType::HealSelf)
		{
			SelfHealSkills.Add(SkillID);
		}

		AnimationsToLoad.Add(Skill->AnimMontage.ToSoftObjectPath());
	}

	if (GEngine)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (GameSingleton)
		{
			if (SkillAnimationsStreamableHandle.IsValid())
			{
				SkillAnimationsStreamableHandle.Get()->ReleaseHandle();
				SkillAnimationsStreamableHandle.Reset();
			}
			SkillAnimationsStreamableHandle = GameSingleton->StreamableManager.RequestSyncLoad(AnimationsToLoad);
		}
	}
	*/
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
