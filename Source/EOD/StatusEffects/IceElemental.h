// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/BaseElemental.h"
#include "IceElemental.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class EOD_API UIceElemental : public UBaseElemental
{
	GENERATED_BODY()
	
public:

	// UIceElemental();

	/**
	 * Called to initialize a status effect on a character.
	 * @param Owner The character that owns the status effect
	 * @param Instigator The actor that initiated the status effect. Can be nullptr. For elemental effects this would be a weapon.
	 */
	// virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Instigator) override;

	/** Called to de-initiate this status effect on a character */
	// virtual void OnDeinitialize() override;

	/** Called when the status effect is activated */
	// virtual void OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters) override;

	/** Called when the status effect is deactivated */
	// virtual void OnDeactivation() override;
	
	// UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	// float SlowDownDuration;
	
	// UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	// float SlowDownModifier;

private:
	
	/** The list of characters that have been slowed down by this status effect */
	static TArray<ABaseCharacter*> SlowedDownCharacters;

	// void ApplySlowDown(ABaseCharacter* TargetCharacter);

	// UFUNCTION()
	// void RemoveSlowDown(ABaseCharacter* TargetCharacter);


};
