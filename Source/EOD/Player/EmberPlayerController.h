// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EmberPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API AEmberPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AEmberPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;
	
};
