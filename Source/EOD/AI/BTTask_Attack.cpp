// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_Attack.h"
#include "Player/EODCharacterBase.h"
#include "Statics/AILibrary.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	if (OwningCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FName SkillID = AIController->GetBlackboardComponent()->GetValueAsName(UAILibrary::BBKey_MostWeightedSkillID);
	bool bResult = OwningCharacter->UseSkill(SkillID);

	if (bResult)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Type();
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	/** The character is supposed to be using most weighted skill during this task */
	FName MostWeightedSkillID = AIController->GetBlackboardComponent()->GetValueAsName(UAILibrary::BBKey_MostWeightedSkillID);

	EEODTaskStatus TaskStatus = OwningCharacter->CheckSkillStatus(MostWeightedSkillID);
	if (TaskStatus == EEODTaskStatus::Active)
	{
		return;
	}
	else if (TaskStatus == EEODTaskStatus::Inactive)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else if (TaskStatus == EEODTaskStatus::Finished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (TaskStatus == EEODTaskStatus::Aborted)
	{
		// FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	return EBTNodeResult::Aborted;
}
