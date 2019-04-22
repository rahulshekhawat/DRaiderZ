// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_Attack.h"
#include "EODCharacterBase.h"
#include "AILibrary.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(CharacterOwner) || !IsValid(BlackboardComp))
	{
		return EBTNodeResult::Failed;
	}

	FName SkillID = BlackboardComp->GetValueAsName(UAILibrary::BBKey_MostWeightedSkillID);
	bool bResult = CharacterOwner->UseSkill(SkillID);
	if (bResult)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(CharacterOwner) || !IsValid(BlackboardComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	/** The character is supposed to be using most weighted skill during this task */
	FName MostWeightedSkillID = BlackboardComp->GetValueAsName(UAILibrary::BBKey_MostWeightedSkillID);
	EEODTaskStatus TaskStatus = CharacterOwner->CheckSkillStatus(MostWeightedSkillID);
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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		//~ @note For some reason, finish latent task with EBTNodeResult::Aborted prevents flow from leaving the task node in behavior tree. I might be missing something.
		// FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	}
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
