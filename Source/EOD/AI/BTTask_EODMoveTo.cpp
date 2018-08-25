// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_EODMoveTo.h"
#include "Player/EODCharacterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_EODMoveTo::OnGameplayTaskActivated(UGameplayTask & Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type UBTTask_EODMoveTo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	EBTNodeResult::Type BTNodeResult = EBTNodeResult::Failed;

	if (OwningCharacter->CanMove())
	{
		BTNodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);
		return BTNodeResult;
	}

	return BTNodeResult;
}

void UBTTask_EODMoveTo::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	// The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	EBTNodeResult::Type BTNodeResult = EBTNodeResult::Failed;

	if (OwningCharacter->CanMove())
	{
		Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
		return;
	}

	FinishLatentTask(OwnerComp, BTNodeResult);
}

void UBTTask_EODMoveTo::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
