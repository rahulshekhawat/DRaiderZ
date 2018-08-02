// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatLibrary.h"
#include "Player/EODCharacterBase.h"
#include "Statics/CombatLibrary.h"

void UCombatLibrary::HandleCombatCollision(AEODCharacterBase * Instigator, UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	if (!IsValid(Instigator))
	{
		return;
	}

	Instigator->HandleMeleeCollision(Animation, HitResults, bHit);
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
