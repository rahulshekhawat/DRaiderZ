// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "GameSingleton.h"
#include "EODPreprocessors.h"
#include "AIStatsComponent.h"
#include "EODWidgetComponent.h"

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
			PlayAnimationMontage(FlinchAnimMontage,
				UCharacterLibrary::SectionName_ForwardFlinch);
		}
		else
		{
			PlayAnimationMontage(FlinchAnimMontage,
				UCharacterLibrary::SectionName_BackwardFlinch);
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
			PlayAnimationMontage(HitEffectsAnimMontage,
				UCharacterLibrary::SectionName_ForwardInterrupt,
				ECharacterState::GotHit);
		}
		else
		{
			PlayAnimationMontage(HitEffectsAnimMontage,
				UCharacterLibrary::SectionName_BackwardInterrupt,
				ECharacterState::GotHit);
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
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AAICharacterBase::EndFreeze, Duration, false);

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
		PlayAnimationMontage(HitEffectsAnimMontage,
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);

		return true;
	}

	return false;
}

void AAICharacterBase::CCEEndKnockdown_Implementation()
{
	PlayAnimationMontage(HitEffectsAnimMontage,
		UCharacterLibrary::SectionName_KnockdownEnd,
		ECharacterState::GotHit);
}

bool AAICharacterBase::CCEKnockback_Implementation(const float Duration, const FVector & ImpulseDirection)
{
	if (CanKnockdown() && HitEffectsAnimMontage)
	{
		PlayAnimationMontage(HitEffectsAnimMontage,
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
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
	if (AnimMontage == FlinchAnimMontage || bUsingUniqueSkill)
	{
		return;
	}

	if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimMontage.Get() == AnimMontage)
	{
		GetLastUsedSkill().LastUsedSkillID = GetCurrentActiveSkillID();
		GetLastUsedSkill().bInterrupted = bInterrupted;

		SetCurrentActiveSkillID(NAME_None);
		SetCurrentActiveSkill(nullptr);
	}

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

bool AAICharacterBase::UseSkill_Implementation(FName SkillID)
{
	if (CanUseAnySkill())
	{
		FSkillTableRow* SkillToUse = GetSkill(SkillID, FString("AAICharacterBase::UseSkill, looking up AI skills for use"));

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
			PlayAnimationMontage(SkillToUse->AnimMontage.Get(), SkillToUse->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		}
		SetCurrentActiveSkillID(SkillID);
		SetCurrentActiveSkill(SkillToUse);
		// SkillIDToWeightMap[SkillID] = SkillIDToWeightMap[SkillID] - 1;
		bSkillAllowsMovement = SkillToUse->bAllowsMovement;
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
}
