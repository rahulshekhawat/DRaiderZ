// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODAIControllerBase.h"
#include "MobController.generated.h"

/**
 * Base class for EOD's AI controller.
 * All AI controller classes for EOD's in-game AI should inherit from this class
 */
UCLASS()
class EOD_API AMobController : public AEODAIControllerBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AMobController(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;





};
