// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/ElementalBase.h"
#include "DarkElemental.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UDarkElemental : public UElementalBase
{
	GENERATED_BODY()
	
public:

	UDarkElemental(const FObjectInitializer& ObjectInitializer);

	virtual TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* GetCharacterToStatusInfoMap() override;

protected:

	/** Called to process the ticking of this status effect. Must be overridden in inherited classes */
	// UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter) override;

private:

	TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> CursedCharactersToStatusInfoMap;	
	
};
