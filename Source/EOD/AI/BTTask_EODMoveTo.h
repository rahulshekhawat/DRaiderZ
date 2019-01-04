// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_EODMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UBTTask_EODMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_EODMoveTo(const FObjectInitializer& ObjectInitializer);

	/** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** ticks this task
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** called when task execution is finished
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;	
	
};
