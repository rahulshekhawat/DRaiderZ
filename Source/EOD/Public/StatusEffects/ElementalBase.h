// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.h"
#include "ElementalBase.generated.h"

class AActor;
class AEODCharacterBase;

/**
 * 
 */
UCLASS()
class EOD_API UElementalBase : public UStatusEffectBase
{
	GENERATED_BODY()

public:

	UElementalBase(const FObjectInitializer& ObjectInitializer);

	/**
	* Called to initialize a status effect on a character.
	* @param Owner The character that owns the status effect
	* @param Instigator The actor that initiated the status effect. Can be nullptr. For a weapon with elemental enchant, the Instigator would be the weapon.
	*/
	virtual void Initialize(AEODCharacterBase* Owner, AActor* Instigator) override;
	
};
