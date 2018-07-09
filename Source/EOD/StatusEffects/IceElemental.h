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

	UIceElemental();

	/** Called to initiate this status effect on a character */
	virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Initiator) override;

	/** Called to de-initiate this status effect on a character */
	virtual void OnDeinitialize() override;

	/** Called when the status effect is activated */
	virtual void OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters) override;

	/** Called when the status effect is deactivated */
	virtual void OnDeactivation() override;
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	float SlowDownDuration;
	
	/**
	 * How much should this status effect slow down the enemy between 0 to 1 ? 
	 * 1 = 100 % , 0 = 0 %
	*/
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	float SlowDownModifier;

private:
	
	/** The characters that have already been inflicted with this status effect */
	static TArray<ABaseCharacter*> AffectedCharacters;

	void ApplySlowDown(ABaseCharacter* TargetCharacter);

	UFUNCTION()
	void RemoveSlowDown(ABaseCharacter* TargetCharacter);


};
