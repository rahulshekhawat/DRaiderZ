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

/** This enum descibes how the hit character responded to an attack */
UENUM(BlueprintType)
enum class ECharacterResponseToDamage : uint8
{
	Immune,
	Blocked,
	Dodged,
	Nullified,
	Damaged
};

/** Struct containing information for damage result */
USTRUCT(BlueprintType)
struct FEODDamageResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	ECharacterResponseToDamage CharacterResponseToDamage;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	ECrowdControlEffect CrowdControlEffect;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	int32 ActualDamage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	bool bCritHit;

	FEODDamageResult()
	{
		CharacterResponseToDamage = ECharacterResponseToDamage::Damaged;
		CrowdControlEffect = ECrowdControlEffect::Flinch;
		ActualDamage = 0;
		bCritHit = false;
	}
};

/** This enum describes the type of damage inflicted */
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Magickal
};

/** Damage info from character skill */
USTRUCT(BlueprintType)
struct FSkillDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	bool bUndodgable;

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	bool bUnblockable;

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	bool bIgnoresBlock;

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	float DamagePercent;

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	EDamageType DamageType;

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	ECrowdControlEffect CrowdControlEffect;

	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillDamageInfo)
	float CrowdControlEffectDuration;
};

USTRUCT(BlueprintType)
struct FEODDamage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	bool bUndodgable;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	bool bUnblockable;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	float CritRate;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	int32 NormalDamage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	int32 CritDamage;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	EDamageType DamageType;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	ECrowdControlEffect CrowdControlEffect;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Damage")
	float CrowdControlEffectDuration;

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

	UCombatLibrary(const FObjectInitializer& ObjectInitializer);

	static FCollisionQueryParams GenerateCombatCollisionQueryParams(const AActor* ActorToIgnore, EQueryMobilityType MobilityType = EQueryMobilityType::Dynamic, bool bReturnPhysicalMaterial = false, FName TraceTag = FName("CollisionQueryForCombat"));

	static float CalculateDamage(float Attack, float Defense);

};
