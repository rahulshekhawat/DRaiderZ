// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "UObject/NoExportTypes.h"
#include "CombatLibrary.generated.h"

class ABaseCharacter;

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

	static void HandleCombatCollision(ABaseCharacter* Instigator, TArray<FHitResult>& CapsuleHitResults, bool bHit);
	
	
	
};
