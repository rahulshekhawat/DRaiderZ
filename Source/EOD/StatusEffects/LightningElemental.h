// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/ElementalBase.h"
#include "LightningElemental.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ULightningElemental : public UElementalBase
{
	GENERATED_BODY()
	
public:

	ULightningElemental(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = LightningElemental)
	float StunDuration;

	/** Called to deinitialize this status effect on a character */
	virtual void Deinitialize() override;

protected:

	/** Called to activate this status effect on a recipient character */
	virtual void ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter);

	/** Called to deactivate this status effect on a recipient character */
	virtual void DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter);
	
};
