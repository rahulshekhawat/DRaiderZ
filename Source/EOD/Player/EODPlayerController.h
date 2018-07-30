// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

/**
 * EmberPlayerController is the base (and final c++) class for EOD's player controller
 * @note All blueprint classes for player controller must inherit from EmberPlayerController
 */
UCLASS()
class EOD_API AEODPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AEODPlayerController(const FObjectInitializer& ObjectInitializer);

	/** Binds functionality for mouse axis input */
	virtual void SetupInputComponent() override;	
	
	
};
