// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"
#include "BaseElemental.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class EOD_API UBaseElemental : public UStatusEffect
{
	GENERATED_BODY()
	
public:
	
	// virtual void OnHit();
	
	/**
	 * If the owning character successfully hits the enemy
	 * @param HitCharacter The enemy character that got hit
	 */
	virtual void OnSuccessfulHit(ABaseCharacter* HitCharacter);
	
	/** If the owning character fails to hit the enemy */
	virtual void OnUnsuccessfulHit();
	
	/** Called when the status effect is activated */
	virtual void OnActivation();

	/** Called when the status effect is deactivated */
	virtual void OnDeactivation();

};
