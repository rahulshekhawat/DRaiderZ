// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "UObject/NoExportTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CombatLibrary.generated.h"

class AEODCharacterBase;
class UAnimSequenceBase;

/** This enum describes all the possible crowd control effects in game */
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

/** This enum descibes the result of damage applied to the hit character */
UENUM(BlueprintType)
enum class EDamageResult : uint8
{
	Immune,			// Character is immune to damage, i.e., character cannot receive any damage as of now
	Nullified,		// Character nullified the incoming damage (e.g., character defense was too high to receive any damage)
	Blocked,		// Character blocked the incoming damage
	Dodged,			// Character dodged the incoming damage
	Damaged			// Character received the incoming damage
};

/** This enum describes the type of damage inflicted */
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Magickal
};

USTRUCT(BlueprintType)
struct EOD_API FReceivedHitInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	AActor* HitInstigator;

	UPROPERTY()
	EDamageResult DamageResult;

	UPROPERTY()
	ECrowdControlEffect CrowdControlEffect;

	UPROPERTY()
	float CrowdControlEffectDuration;

	UPROPERTY()
	float BCAngle;

	/** Amount of actual damage that the character received */
	UPROPERTY()
	int32 ActualDamage;

	/** True if the character received a critical hit */
	UPROPERTY()
	bool bCritHit;

	UPROPERTY()
	FVector_NetQuantize HitLocation;

	UPROPERTY()
	uint8 ReplicationIndex;

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> HitSurface;

	FReceivedHitInfo() :
		HitInstigator(nullptr),
		DamageResult(EDamageResult::Damaged),
		CrowdControlEffect(ECrowdControlEffect::Flinch),
		CrowdControlEffectDuration(0.f),
		BCAngle(0.f),
		ActualDamage(0.f),
		bCritHit(false),
		ReplicationIndex(0),
		HitSurface(EPhysicalSurface::SurfaceType_Default)
	{
	}
};

/** This struct contains information of how the character received damage */
USTRUCT(BlueprintType)
struct FAttackResponse
{
	GENERATED_USTRUCT_BODY()

	/** Result of damage applied to the character */
	UPROPERTY(BlueprintReadWrite)
	EDamageResult DamageResult;

	/** The crowd control effect that was applied to the damaged character */
	UPROPERTY(BlueprintReadWrite)
	ECrowdControlEffect CrowdControlEffect;

	/** Amount of actual damage that the character received */
	UPROPERTY(BlueprintReadWrite)
	int32 ActualDamage;

	/** True if the character received a critical hit */
	UPROPERTY(BlueprintReadWrite)
	bool bCritHit;

	FAttackResponse() :
		DamageResult(EDamageResult::Damaged),
		CrowdControlEffect(ECrowdControlEffect::Flinch),
		ActualDamage(0),
		bCritHit(false)
	{
	}

	FAttackResponse(EDamageResult Result, ECrowdControlEffect CCEffect, int32 Damage, bool bCritical) :
		DamageResult(Result),
		CrowdControlEffect(CCEffect),
		ActualDamage(Damage),
		bCritHit(bCritical)
	{
	}
};

/** This struct contains information of current attack */
USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_USTRUCT_BODY()

	/** True if this attack cannot be dodged */
	UPROPERTY(BlueprintReadWrite)
	bool bUndodgable;

	/** True if this attack cannot be blocked */
	UPROPERTY(BlueprintReadWrite)
	bool bUnblockable;

	UPROPERTY(BlueprintReadWrite)
	float CritRate;

	UPROPERTY(BlueprintReadWrite)
	float NormalDamage;

	UPROPERTY(BlueprintReadWrite)
	float CritDamage;

	UPROPERTY(BlueprintReadWrite)
	EDamageType DamageType;

	UPROPERTY(BlueprintReadWrite)
	ECrowdControlEffect CrowdControlEffect;

	UPROPERTY(BlueprintReadWrite)
	float CrowdControlEffectDuration;

	FAttackInfo() :
		bUndodgable(false),
		bUnblockable(false),
		CritRate(0.f),
		NormalDamage(0.f),
		CritDamage(0.f),
		DamageType(EDamageType::Physical),
		CrowdControlEffect(ECrowdControlEffect::Flinch),
		CrowdControlEffectDuration(0.f)
	{
	}

	FAttackInfo(bool bInUndodgable, bool bInUnblockable, float InCritRate, float InNormalDamage, float InCritDamage, EDamageType InDamageType, ECrowdControlEffect InCCEffect, float InCCEDuration) :
		bUndodgable(bInUndodgable),
		bUnblockable(bInUnblockable),
		CritRate(InCritRate),
		NormalDamage(InNormalDamage),
		CritDamage(InCritDamage),
		DamageType(InDamageType),
		CrowdControlEffect(InCCEffect),
		CrowdControlEffectDuration(InCCEDuration)
	{
	}
};

USTRUCT()
struct EOD_API FMeleeDamageEvent : public FDamageEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Damage;

	/** True if this attack cannot be dodged */
	UPROPERTY()
	bool bUndodgable;

	/** True if this attack cannot be blocked */
	UPROPERTY()
	bool bUnblockable;

	UPROPERTY()
	ECrowdControlEffect CrowdControlEffect;

	UPROPERTY()
	float CrowdControlEffectDuration;

	static const int32 ClassID = 11;

	virtual int32 GetTypeID() const override { return FMeleeDamageEvent::ClassID; }
	virtual bool IsOfType(int32 InID) const override { return FMeleeDamageEvent::ClassID == InID; }

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

	static FCollisionQueryParams GenerateCombatCollisionQueryParams(const AActor* ActorToIgnore, EQueryMobilityType MobilityType = EQueryMobilityType::Dynamic, bool bReturnPhysicalMaterial = true, FName TraceTag = FName("CollisionQueryForCombat"));

	static float CalculateDamage(float Attack, float Defense);

	static const float PhysicalCritMultiplier;
	static const float MagickalCritMultiplier;

	static const float BlockDetectionAngle;

};
