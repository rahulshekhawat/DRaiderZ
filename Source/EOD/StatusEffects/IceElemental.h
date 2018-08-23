// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/ElementalBase.h"
#include "IceElemental.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class EOD_API UIceElemental : public UElementalBase
{
	GENERATED_BODY()
	
public:

	UIceElemental(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(EditDefaultsOnly, Category = IceElementalInfo)
	float SlowDownModifier;

	/** Called to activate this status effect on a recipient character */
	virtual void ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter);

	/** Called to process the ticking of this status effect. Must be overridden in inherited classes */
	// UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter) override;



};
