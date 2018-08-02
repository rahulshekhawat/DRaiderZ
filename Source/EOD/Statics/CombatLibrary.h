// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "UObject/NoExportTypes.h"
#include "CombatLibrary.generated.h"

class AEODCharacterBase;
class UAnimSequenceBase;

/** This enum describes all the possible crowd control effects in-game */
UENUM(BlueprintType, Meta = (BitFlags))
enum class ECrowdControlEffect : uint8
{
	Flinch,
	Interrupt,
	KnockedDown,
	KnockedBack,
	Stunned,
	Crystalized
};

/**
 * CombatLibrary contains static helper functions for handling in-game combat.
 * @note Do not derive from this class
 */
UCLASS()
class EOD_API UCombatLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	static void HandleCombatCollision(AEODCharacterBase* Instigator, UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);
	
	static void HandleCombatCollision(AActor* Instigator, UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);

	static FCollisionQueryParams GenerateCombatCollisionQueryParams(AActor* ActorToIgnore, EQueryMobilityType MobilityType = EQueryMobilityType::Dynamic, bool bReturnPhysicalMaterial = false, FName TraceTag = FName("CollisionQueryForCombat"));


};
