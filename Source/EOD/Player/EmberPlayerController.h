// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EmberPlayerController.generated.h"

/**
 * EmberPlayerController is the base (and final c++) class for EOD's player controller
 * @note All blueprint classes for player controller must inherit from EmberPlayerController
 */
UCLASS()
class EOD_API AEmberPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AEmberPlayerController(const FObjectInitializer& ObjectInitializer);

	/** Binds functionality for mouse axis input */
	virtual void SetupInputComponent() override;
	
};
