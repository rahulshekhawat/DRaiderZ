// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatLibrary.h"
#include "CharacterLibrary.h"
#include "Player/EODCharacterBase.h"

void UCombatLibrary::HandleCombatCollision(AEODCharacterBase * Instigator, UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	if (!IsValid(Instigator))
	{
		return;
	}

	Instigator->OnMeleeCollision(Animation, HitResults, bHit);
	// FActiveSkill ActiveSkill = Instigator->GetCurrentActiveSkillInfo();
	
}

void UCombatLibrary::HandleCombatCollision(AActor * Instigator, UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit)
{
}

FCollisionQueryParams UCombatLibrary::GenerateCombatCollisionQueryParams(AActor * ActorToIgnore, EQueryMobilityType MobilityType, bool bReturnPhysicalMaterial, FName TraceTag)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ActorToIgnore);
	Params.bIgnoreTouches = true;
	Params.bIgnoreBlocks = false;
	Params.bReturnFaceIndex = false;
	Params.bReturnPhysicalMaterial = bReturnPhysicalMaterial;
	Params.MobilityType = MobilityType;
	Params.bTraceComplex = false;
	Params.TraceTag = TraceTag;
	// @todo mask filter for teams.

	return Params;
}

FEODDamage::FEODDamage(FSkill * Skill)
{
	if (Skill)
	{
		DamageType				= Skill->DamageType;
		bUndodgable				= Skill->SkillLevelUpInfo.bUndodgable;
		bUnblockable			= Skill->SkillLevelUpInfo.bUnblockable;
		DamagePercent			= Skill->SkillLevelUpInfo.DamagePercent;
		CrowdControlEffect		= Skill->SkillLevelUpInfo.CrowdControlEffect;		
	}
	else
	{
		FEODDamage();
	}
}
