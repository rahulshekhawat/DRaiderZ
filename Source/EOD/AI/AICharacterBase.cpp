// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "Core/GameSingleton.h"
#include "Core/EODPreprocessors.h"
#include "Statics/CharacterLibrary.h"
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

	// Make sure DataTable_Skills is not nullptr
	check(DataTable_Skills);

	AggroWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
	HealthWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);

	TArray<FName> SkillIDs = DataTable_Skills->GetRowNames();
	for (const FName& SkillID : SkillIDs)
	{
		FAISkillTableRow* AISkill = DataTable_Skills->FindRow<FAISkillTableRow>(SkillID, FString("Looking up AI skill"), false);
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


	// AnimMontage_HitEffects->IsValidSectionName()


	// AnimMontage_HitEffects->


	// UCharacterLibrary::GetAllAICharacterSkills(InGameName, DataTable_Skills, Skills);

	// Initialize skills and load animation montages


}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetInCombat(false);

	// @note for some reason disabling tick hides the widget component. Maybe it's because it needs to be drawn every tick?
	// AggroWidgetComp->SetComponentTickEnabled(false);
	// HealthWidgetComp->SetComponentTickEnabled(false);

}

void AAICharacterBase::Destroyed()
{
	/*
	for (FSkill* Skill : Skills)
	{
		delete Skill;
		Skill = nullptr;
	}

	Skills.Empty();
	*/
}

UEODWidgetComponent * AAICharacterBase::BP_GetAggroWidgetComp() const
{
	return GetAggroWidgetComp();
}

UEODWidgetComponent * AAICharacterBase::BP_GetHealthWidgetComp() const
{
	return GetHealthWidgetComp();
}

void AAICharacterBase::Interrupt(const EHitDirection InterruptDirection)
{
	if (InterruptDirection == EHitDirection::Forward)
	{
		PlayAnimationMontage(AnimMontage_HitEffects,
			UCharacterLibrary::SectionName_ForwardInterrupt,
			ECharacterState::GotHit);
	}
	else if (InterruptDirection == EHitDirection::Backward)
	{
		PlayAnimationMontage(AnimMontage_HitEffects,
			UCharacterLibrary::SectionName_BackwardInterrupt,
			ECharacterState::GotHit);
	}
}

void AAICharacterBase::Flinch(const EHitDirection FlinchDirection)
{
	if (FlinchDirection == EHitDirection::Forward)
	{
		PlayAnimationMontage(AnimMontage_Flinch,
			UCharacterLibrary::SectionName_ForwardFlinch);
	}
	else if (FlinchDirection == EHitDirection::Backward)
	{
		PlayAnimationMontage(AnimMontage_Flinch,
			UCharacterLibrary::SectionName_BackwardFlinch);
	}
}

void AAICharacterBase::Stun(const float Duration)
{
}

void AAICharacterBase::EndStun()
{
}

void AAICharacterBase::Freeze(const float Duration)
{
}

void AAICharacterBase::EndFreeze()
{
}

void AAICharacterBase::Knockdown(const float Duration)
{
}

void AAICharacterBase::EndKnockdown()
{
}

void AAICharacterBase::Knockback(const float Duration, const FVector & Impulse)
{
}

void AAICharacterBase::SetInCombat(bool bValue)
{
	bInCombat = bValue;
	UpdateMaxWalkSpeed();
}

void AAICharacterBase::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	FAISkillTableRow* AISkill = nullptr;
	if (GetCurrentActiveSkillID() != NAME_None)
	{
		AISkill = DataTable_Skills->FindRow<FAISkillTableRow>(GetCurrentActiveSkillID(), FString("Looking up AI skill"));
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
}

void AAICharacterBase::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
	// @todo
}

bool AAICharacterBase::UseSkill(FName SkillID)
{
	if (CanUseAnySkill())
	{
		FAISkillTableRow* SkillToUse = DataTable_Skills->FindRow<FAISkillTableRow>(SkillID, FString("Looking up AI skill for use"));

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

FName AAICharacterBase::GetMostWeightedMeleeSkillID(AEODCharacterBase const * const TargetCharacter) const
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

void AAICharacterBase::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;	
}
