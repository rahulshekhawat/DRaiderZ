// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AICharacterBase.h"
#include "Core/GameSingleton.h"
#include "Statics/CharacterLibrary.h"
#include "Components/AIStatsComponent.h"
#include "Player/Components/EODWidgetComponent.h"

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

void AAICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UCharacterLibrary::GetAllAICharacterSkills(InGameName, DataTable_Skills, Skills);

	// Initialize skills and load animation montages

	

}

void AAICharacterBase::Destroyed()
{
	for (FSkill* Skill : Skills)
	{
		delete Skill;
		Skill = nullptr;
	}

	Skills.Empty();
}

UEODWidgetComponent * AAICharacterBase::BP_GetAggroWidgetComp() const
{
	return GetAggroWidgetComp();
}

UEODWidgetComponent * AAICharacterBase::BP_GetHealthWidgetComp() const
{
	return GetHealthWidgetComp();
}

void AAICharacterBase::SetInCombat(bool bValue)
{
	bInCombat = bValue;
	UpdateMaxWalkSpeed();
}


void AAICharacterBase::OnMeleeCollision(UAnimSequenceBase * Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	Super::OnMeleeCollision(Animation, HitResults, bHit);
}

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

void AAICharacterBase::Interrupt(const EHitDirection InterruptDirection)
{
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

void AAICharacterBase::UpdateMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bInCombat ? MaxWalkSpeedInCombat : MaxWalkSpeedOutsideCombat;	
}
