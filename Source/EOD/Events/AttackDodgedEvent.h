// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Events/GameplayEventBase.h"
#include "AttackDodgedEvent.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAttackDodgedEvent : public UGameplayEventBase
{
	GENERATED_BODY()

public:

	UAttackDodgedEvent(const FObjectInitializer& ObjectInitializer);
	
};
