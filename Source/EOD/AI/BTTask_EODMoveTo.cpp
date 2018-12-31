// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_EODMoveTo.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EODMoveTo::UBTTask_EODMoveTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "EOD Move To";
}

void UBTTask_EODMoveTo::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

EBTNodeResult::Type UBTTask_EODMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;

	EBTNodeResult::Type BTNodeResult = EBTNodeResult::Failed;
	if (IsValid(CharacterOwner) && CharacterOwner->CanMove())
	{
		BTNodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);
		return BTNodeResult;
	}
	return BTNodeResult;
}

void UBTTask_EODMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;

	EBTNodeResult::Type BTNodeResult = EBTNodeResult::Failed;
	if (IsValid(CharacterOwner) && CharacterOwner->CanMove())
	{
		Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
		return;
	}
	FinishLatentTask(OwnerComp, BTNodeResult);
}

void UBTTask_EODMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
