// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffect.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS(Abstract)
class EOD_API UStatusEffect : public UObject
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

	ABaseCharacter* GetOwningCharacter() const;

	void SetOwningCharacter(ABaseCharacter* NewCharacter);

private:

	ABaseCharacter* OwningCharacter;
	
	// @todo Add flags to determine allies and enemies (for buff and debuff effects)
	// int AllyFlag;
	// @todo add boolean to check if this status effects deactives on death
	// @todo add boolean to determine whether the status effect is currently active or not
	
};
