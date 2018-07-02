// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/BaseElemental.h"
#include "IceElemental.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UIceElemental : public UBaseElemental
{
	GENERATED_BODY()
	
public:

	UIceElemental();

	/**
	* If the owner of status effect received any damage
	* @param HittingCharacter The enemy character that landed a hit on you
	*/
	virtual void OnReceivingHit(ABaseCharacter* HittingCharacter) override;

	/**
	* If the owning character successfully hits the enemy
	* @param HitCharacter The enemy character that got hit
	*/
	virtual void OnSuccessfulHit(ABaseCharacter* HitCharacter) override;

	/** If the owning character fails to hit the enemy */
	virtual void OnUnsuccessfulHit() override;

	/** Called to initiate this status effect on a character */
	virtual void OnInitialize(class ABaseCharacter* Owner) override;

	/** Called to de-initiate this status effect on a character */
	virtual void OnDeinitialize() override;

	/** Called when the status effect is activated */
	virtual void OnActivation() override;

	/** Called when the status effect is deactivated */
	virtual void OnDeactivation() override;
	
	
};
