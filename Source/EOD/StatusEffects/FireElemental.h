// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/BaseElemental.h"
#include "FireElemental.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UFireElemental : public UBaseElemental
{
	GENERATED_BODY()
	
public:

	// UFireElemental();
	
	/**
	 * Called to initialize a status effect on a character.
	 * @param Owner The character that owns the status effect
	 * @param Instigator The actor that initiated the status effect. Can be nullptr. For elemental effects this would be a weapon.
	 */
	// virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Instigator) override;

	/** Called to deinitialize this status effect on a character */
	// virtual void OnDeinitialize() override;

	/** Called when the status effect is activated */
	// virtual void OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters) override;

	/** Called when the status effect is deactivated */
	// virtual void OnDeactivation() override;

private:

	
	
	
};
