// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/StatusEffects/StatusEffectBase.h"
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
	
protected:

	virtual void OnStatusEffectTick_Implementation(AEODCharacterBase* TargetCharacter) override;

};
