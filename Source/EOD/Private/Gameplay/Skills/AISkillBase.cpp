// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AISkillBase.h"
#include "AIStatsComponent.h"
#include "AICharacterBase.h"
#include "EODAIControllerBase.h"
#include "GameplaySkillsComponent.h"

UAISkillBase::UAISkillBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSkillCanBeCharged					= false;
	AnimationStartSectionName			= FName("Default");
	CamShakeType						= ECameraShakeType::Weak;
}

bool UAISkillBase::CanTriggerSkill() const
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();

	if (Instigator)
	{
		bool bInstigatorCanUseSkill = Instigator->IsIdleOrMoving() || Instigator->IsBlocking() || Instigator->IsNormalAttacking();
		return bInstigatorCanUseSkill && SkillMontage;
	}

	return false;
}

void UAISkillBase::TriggerSkill()
{
}

bool UAISkillBase::CanReleaseSkill() const
{
	return false;
}

void UAISkillBase::FinishSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator)
	{
		Instigator->ResetState();
	}

	UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
	if (SkillsComp)
	{
		SkillsComp->OnSkillFinished(SkillIndex, SkillGroup, this);
	}
}

TSharedPtr<FAttackInfo> UAISkillBase::GetAttackInfoPtr(int32 CollisionIndex)
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (Instigator->Role < ROLE_Authority)
	{
		return TSharedPtr<FAttackInfo>();
	}

	AEODAIControllerBase* AIC = Instigator ? Cast<AEODAIControllerBase>(Instigator->Controller) : nullptr;
	UAIStatsComponent* StatsComp = AIC ? AIC->GetStatsComponent() : nullptr;
	check(StatsComp);

	float CritRate = DamageType == EDamageType::Magickal ? StatsComp->MagickalCritRate.GetValue() : StatsComp->PhysicalCritRate.GetValue();
	float NormalDamage =
		DamageType == EDamageType::Magickal ?
		((SkillInfo.DamagePercent / 100.f) * StatsComp->MagickalAttack.GetValue()) :
		((SkillInfo.DamagePercent / 100.f) * StatsComp->PhysicalAttack.GetValue());
	float CritDamage =
		DamageType == EDamageType::Magickal ?
		(NormalDamage * UCombatLibrary::MagickalCritMultiplier + StatsComp->MagickalCritBonus.GetValue()) :
		(NormalDamage * UCombatLibrary::PhysicalCritMultiplier + StatsComp->PhysicalCritBonus.GetValue());

	TSharedPtr<FAttackInfo> AttackInfoPtr =
		MakeShareable(new FAttackInfo(
			SkillInfo.bUndodgable,
			SkillInfo.bUnblockable,
			CritRate,
			NormalDamage,
			CritDamage,
			DamageType,
			SkillInfo.CrowdControlEffect,
			SkillInfo.CrowdControlEffectDuration,
			CamShakeType
		));

	return AttackInfoPtr;
}
