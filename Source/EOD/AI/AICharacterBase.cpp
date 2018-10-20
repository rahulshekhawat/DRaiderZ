// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "Core/GameSingleton.h"
#include "Core/EODPreprocessors.h"
#include "Components/AIStatsComponent.h"
#include "Player/Components/EODWidgetComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AAICharacterBase::AAICharacterBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UAIStatsComponent>(FName("Character Stats Component")))
{
	// Mob characters don't have strafe animations and so they must be rotated in the direction of their movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AggroWidgetComp = ObjectInitializer.CreateDefaultSubobject<UEODWidgetComponent>(this, TEXT("Aggro Indicator"));
	AggroWidgetComp->SetupAttachment(RootComponent);
	HealthWidgetComp = ObjectInitializer.CreateDefaultSubobject<UEODWidgetComponent>(this, TEXT("Health Indicator"));
	HealthWidgetComp->SetupAttachment(RootComponent);

	AggroWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AggroWidgetComp->SetCollisionProfileName(FName("NoCollision"));
	HealthWidgetComp->SetCollisionProfileName(FName("NoCollision"));
}

void AAICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// AggroWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	// HealthWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);

}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetInCombat(false);

	/*
	check(SkillsDataTable);;

	TArray<FName> SkillIDs = SkillsDataTable->GetRowNames();
	for (const FName& SkillID : SkillIDs)
	{
		FAISkillTableRow* AISkill = SkillsDataTable->FindRow<FAISkillTableRow>(SkillID, FString("Looking up AI skill"), false);
		check(AISkill);

		if (AISkill->SkillType == ESkillType::BuffParty)
		{
			PartyBuffSkills.Add(SkillID);
		}
		else if (AISkill->SkillType == ESkillType::BuffSelf)
		{
			SelfBuffSkills.Add(SkillID);
		}
		else if (AISkill->SkillType == ESkillType::DamageMelee)
		{
			MeleeSkills.Add(SkillID);
			if (AISkill->CrowdControlEffect == ECrowdControlEffect::Crystalized)
			{
				CrystalizeSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::Flinch)
			{
				FlinchSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::Interrupt)
			{
				InterruptSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::KnockedBack)
			{
				KnockBackSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::KnockedDown)
			{
				KnockDownSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::Stunned)
			{
				StunSkills.Add(SkillID);
			}
		}
		else if (AISkill->SkillType == ESkillType::DamageRanged)
		{
			RangedSkills.Add(SkillID);
			if (AISkill->CrowdControlEffect == ECrowdControlEffect::Crystalized)
			{
				CrystalizeSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::Flinch)
			{
				FlinchSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::Interrupt)
			{
				InterruptSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::KnockedBack)
			{
				KnockBackSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::KnockedDown)
			{
				KnockDownSkills.Add(SkillID);
			}
			else if (AISkill->CrowdControlEffect == ECrowdControlEffect::Stunned)
			{
				StunSkills.Add(SkillID);
			}
		}
		else if (AISkill->SkillType == ESkillType::DebuffEnemy)
		{
			DebuffSkills.Add(SkillID);
		}
		else if (AISkill->SkillType == ESkillType::HealParty)
		{
			PartyHealSkills.Add(SkillID);
		}
		else if (AISkill->SkillType == ESkillType::HealSelf)
		{
			SelfHealSkills.Add(SkillID);
		}

		SkillIDToWeightMap.Add(SkillID, 0);
	}
	*/
}

void AAICharacterBase::Destroyed()
{
}

UEODWidgetComponent * AAICharacterBase::BP_GetAggroWidgetComp() const
{
	return GetAggroWidgetComp();
}

UEODWidgetComponent * AAICharacterBase::BP_GetHealthWidgetComp() const
{
	return GetHealthWidgetComp();
}

bool AAICharacterBase::Interrupt(const float BCAngle)
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

bool AAICharacterBase::Flinch(const float BCAngle)
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

bool AAICharacterBase::Stun(const float Duration)
{
	// @todo definition

	return false;
}

void AAICharacterBase::EndStun()
{
}

bool AAICharacterBase::Freeze(const float Duration)
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

void AAICharacterBase::EndFreeze()
{
	// CustomTimeDilation = StatsComp->GetActiveTimeDilation();
}

bool AAICharacterBase::Knockdown(const float Duration)
{
	return false;
}

void AAICharacterBase::EndKnockdown()
{
}

bool AAICharacterBase::Knockback(const float Duration, const FVector & Impulse)
{
	return false;
}

void AAICharacterBase::SetInCombat(bool bValue)
{
	bInCombat = bValue;
	UpdateMaxWalkSpeed();
}

void AAICharacterBase::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	/*
	FAISkillTableRow* AISkill = nullptr;
	if (GetCurrentActiveSkillID() != NAME_None)
	{
		AISkill = SkillsDataTable->FindRow<FAISkillTableRow>(GetCurrentActiveSkillID(), FString("Looking up AI skill"));
	}

	if (AISkill && AISkill->AnimMontage == AnimMontage)
	{
		GetLastUsedSkill().LastUsedSkillID = GetCurrentActiveSkillID();
		GetLastUsedSkill().bInterrupted = bInterrupted;

		// @todo maybe put following code in a function called ResetState
		SetCharacterState(ECharacterState::IdleWalkRun);
		SetCurrentActiveSkillID(NAME_None);
	}
	else
	{
	}
	*/
}

void AAICharacterBase::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
	// @todo
}

bool AAICharacterBase::UseSkill(FName SkillID)
{
	/*
	if (CanUseAnySkill())
	{
		FAISkillTableRow* SkillToUse = SkillsDataTable->FindRow<FAISkillTableRow>(SkillID, FString("Looking up AI skill for use"));

		if (!SkillToUse)
		{
			UKismetSystemLibrary::PrintString(this, FString("Couldn't find the skill AI character is trying to use"));
			return false;
		}

		PlayAnimationMontage(SkillToUse->AnimMontage, SkillToUse->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		SetCurrentActiveSkillID(SkillID);
		SkillIDToWeightMap[SkillID] = SkillIDToWeightMap[SkillID] - 1;
		return true;
	}
	*/

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

			if (SkillIDToWeightMap[SkillID] > SkillIDToWeightMap[MostWeightedSkillID])
			{
				MostWeightedSkillID = SkillID;
			}
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

			if (SkillIDToWeightMap[SkillID] > SkillIDToWeightMap[MostWeightedSkillID])
			{
				MostWeightedSkillID = SkillID;
			}
		}
	}

	return MostWeightedSkillID;
}

/*
FSkillDamageInfo AAICharacterBase::GetCurrentActiveSkillDamageInfo() const
{
	FSkillDamageInfo SkillDamageInfo;
	FString ContextString = FString("AAICharacterBase::GetCurrentActiveSkillDamageInfo(), looking for AI skill");
	FAISkillTableRow* Skill = SkillsDataTable->FindRow<FAISkillTableRow>(GetCurrentActiveSkillID(), ContextString);
	SkillDamageInfo.bUnblockable = Skill->bUnblockable;
	SkillDamageInfo.bUndodgable = Skill->bUndodgable;
	SkillDamageInfo.bIgnoresBlock = Skill->bIgnoresBlock;
	SkillDamageInfo.CrowdControlEffect = Skill->CrowdControlEffect;
	SkillDamageInfo.CrowdControlEffectDuration = Skill->CrowdControlEffectDuration;
	SkillDamageInfo.DamagePercent = Skill->DamagePercent;
	SkillDamageInfo.DamageType = Skill->DamageType;

	return SkillDamageInfo;
}
*/

void AAICharacterBase::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;	
}
