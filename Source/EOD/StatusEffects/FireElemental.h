// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/ElementalBase.h"
#include "FireElemental.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UFireElemental : public UElementalBase
{
	GENERATED_BODY()
	
public:

	UFireElemental(const FObjectInitializer& ObjectInitializer);

	virtual TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* GetCharacterToStatusInfoMap() override;

protected:

	/** Called to process the ticking of this status effect. Must be overridden in inherited classes */
	// UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter) override;

private:

	static TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> BurningCharactersToStatusInfoMap;

};
