// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Events/EODEventBase.h"
#include "GameplayEventBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UGameplayEventBase : public UEODEventBase
{
	GENERATED_BODY()

public:

	UGameplayEventBase(const FObjectInitializer& ObjectInitializer);
	
};
