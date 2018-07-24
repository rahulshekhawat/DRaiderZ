// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatLibrary.h"

void UCombatLibrary::HandleCombatCollision(ABaseCharacter * Instigator, UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit)
{
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
