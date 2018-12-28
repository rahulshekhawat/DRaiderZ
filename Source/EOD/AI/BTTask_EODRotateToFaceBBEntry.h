// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_EODRotateToFaceBBEntry.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UBTTask_EODRotateToFaceBBEntry : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_EODRotateToFaceBBEntry(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:

	/** Success condition precision in degrees */
	UPROPERTY(EditAnywhere, Category = Node, meta = (ClampMin = "0.0"))
	float Precision;
	
};
