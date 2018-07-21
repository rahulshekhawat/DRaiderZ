// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageLibrary.generated.h"


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
 * DamageLibrary contains static helper functions for handling in-game damage events.
 * @note Do not derive from this class
 */
UCLASS()
class EOD_API UDamageLibrary : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
