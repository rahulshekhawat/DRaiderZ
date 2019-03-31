// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MobController.generated.h"

/**
 * Base class for EOD's AI controller.
 * All AI controller classes for EOD's in-game AI should inherit from this class
 */
UCLASS()
class EOD_API AMobController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMobController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = Blackboard)
	void InitializeBlackboardValues(class UBlackboardComponent* BlackboardComponent);

private:
	UPROPERTY(EditDefaultsOnly, Category = BlackboardValues)
	float AggroActivationRadius;

	UPROPERTY(EditDefaultsOnly, Category = BlackboardValues)
	float AggroAreaRadius;

	UPROPERTY(EditDefaultsOnly, Category = BlackboardValues)
	float MaxEnemyChaseRadius;

	UPROPERTY(EditDefaultsOnly, Category = BlackboardValues)
	float WanderRadius;


};
