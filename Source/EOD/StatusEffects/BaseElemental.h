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

	/** Called to initiate this status effect on a character */
	virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Initiator) override;

	/** Called to de-initiate this status effect on a character */
	virtual void OnDeinitialize() override;

	/** Called when the status effect is activated */
	virtual void OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters) override;

	/** Called when the status effect is deactivated */
	virtual void OnDeactivation() override;

};
