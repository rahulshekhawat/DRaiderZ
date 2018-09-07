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

UENUM(BlueprintType)
enum class ECharacterResponseToDamage : uint8
{
	Immune,
	Blocked,
	Dodged,
	Nullified,
	Damaged
};

USTRUCT(BlueprintType)
struct FEODDamageResult
{
	GENERATED_USTRUCT_BODY()

public:

	ECharacterResponseToDamage CharacterResponseToDamage;

	ECrowdControlEffect CrowdControlEffect;

	uint32 ActualDamage;

	FEODDamageResult()
	{
		CharacterResponseToDamage = ECharacterResponseToDamage::Damaged;
		ActualDamage = 0;
	}

};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Magickal
};

/** In-game skill */
USTRUCT(BlueprintType)
struct FEODDamage
{
	GENERATED_USTRUCT_BODY()

public:

	/** Actual collision hit result */
	FHitResult CollisionHitResult;

	/** 
	 * Line hit result from instigator to hit character.
	 * Impact location and impact normal are used to play hit sound, hit particle effect, and determining whether hit character blocked the attack
	 */
	FHitResult LineHitResult;
	
	/** The character that caused damage */
	AEODCharacterBase* Instigator;

	/** Determines whether the hit causes critical damage or not */
	bool bCriticalHit;

	FEODDamage()
	{
		bCriticalHit = false;
	}
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

	static float CalculateDamage(float Attack, float Defense);

	static int PhysicalCritMultiplier;

	static int MagickalCritMultiplier;

};
