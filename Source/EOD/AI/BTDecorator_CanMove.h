// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanMove.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UBTDecorator_CanMove : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTDecorator_CanMove(const FObjectInitializer & ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
	
};
