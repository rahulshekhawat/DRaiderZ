// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "Core/GameSingleton.h"
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

			UKismetSystemLibrary::PrintString(this, FString("Found melee skill"));
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

	AggroWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
	HealthWidgetComp->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);

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
	/*
	if (InterruptDirection == EHitDirection::Forward)
	{
		PlayAnimationMontage(AnimationMontage_HitEffects,
			UCharacterLibrary::SectionName_ForwardInterrupt,
			ECharacterState::GotHit);
	}
	else if (InterruptDirection == EHitDirection::Backward)
	{
		PlayAnimationMontage(AnimationMontage_HitEffects,
			UCharacterLibrary::SectionName_BackwardInterrupt,
			ECharacterState::GotHit);
	}
	*/
}

void AAICharacterBase::Flinch(const EHitDirection FlinchDirection)
{
	/*
	if (FlinchDirection == EHitDirection::Forward)
	{

		PlayerAnimInstance->Montage_Play(GetActiveAnimationReferences()->AnimationMontage_Flinch);
		PlayerAnimInstance->Montage_JumpToSection(UCharacterLibrary::SectionName_ForwardFlinch,
			GetActiveAnimationReferences()->AnimationMontage_Flinch);
	}
	else if (FlinchDirection == EHitDirection::Backward)
	{
		PlayerAnimInstance->Montage_Play(GetActiveAnimationReferences()->AnimationMontage_Flinch);
		PlayerAnimInstance->Montage_JumpToSection(UCharacterLibrary::SectionName_BackwardFlinch,
			GetActiveAnimationReferences()->AnimationMontage_Flinch);
	}
	*/
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

/*
void AAICharacterBase::OnMeleeCollision(UAnimSequenceBase * Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	Super::OnMeleeCollision(Animation, HitResults, bHit);
}
*/

void AAICharacterBase::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
	/*
	if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimationMontage == AnimMontage && !bInterrupted)
	{
		GetLastUsedSkill().LastUsedSkill = CurrentActiveSkill;
		GetLastUsedSkill().bInterrupted = bInterrupted;
		CurrentActiveSkill = nullptr;

		// Revert back to IdleWalkRun state
		CharacterState = ECharacterState::IdleWalkRun;
	}
	*/
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
		return true;
	}


	/*
	if (CanUseAnySkill())
	{
		FSkill* SkillToUse = GetSkill(SkillIndex);

		if (!SkillToUse)
		{
			// unable to use skill - return false
			return false;
		}

		PlayAnimationMontage(SkillToUse->AnimationMontage_GenderOne, SkillToUse->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		CurrentActiveSkill = SkillToUse;
		return true;
	}
	*/
	return false;
}

FName AAICharacterBase::GetMostWeightedMeleeSkillID(AEODCharacterBase const * const TargetCharacter) const
{
	if (MeleeSkills.Num() > 0)
	{
		return MeleeSkills[0];
	}

	return FName();
}

void AAICharacterBase::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;	
}
