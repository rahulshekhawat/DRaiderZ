// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffectBase.h"
#include "FuriousDashBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UFuriousDashBase : public UStatusEffectBase
{
	GENERATED_BODY()
	
public:

	UFuriousDashBase(const FObjectInitializer& ObjectInitializer);

	virtual void OnTriggerEvent_Implementation(AEODCharacterBase* RecipientCharacter) override;
	
	
};
