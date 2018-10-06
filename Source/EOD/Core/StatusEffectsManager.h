// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "StatusEffectsManager.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API AStatusEffectsManager : public AInfo
{
	GENERATED_BODY()
	
public:

	AStatusEffectsManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	
	
};
