// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_EODMoveTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_EODMoveTo::OnGameplayTaskActivated(UGameplayTask & Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type UBTTask_EODMoveTo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTTask_EODMoveTo::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_EODMoveTo::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
