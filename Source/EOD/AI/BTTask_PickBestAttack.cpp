// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_PickBestAttack.h"
#include "EOD/Player/EODCharacterBase.h"
#include "EOD/Statics/AILibrary.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTask_PickBestAttack::UBTTask_PickBestAttack(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
    NodeName = "Pick Best Attack";
    bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_PickBestAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
    AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	if (OwningCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UObject* KeyValue = OwnerComp.GetBlackboardComponent()->GetValueAsObject(UAILibrary::BBKey_TargetEnemy);
	AEODCharacterBase* TargetCharacter = Cast<AEODCharacterBase>(KeyValue);

	FName MostWeightedSkillID = OwningCharacter->GetMostWeightedMeleeSkillID(TargetCharacter);
	OwnerComp.GetBlackboardComponent()->SetValueAsName(UAILibrary::BBKey_MostWeightedSkillID, MostWeightedSkillID);

	if (MostWeightedSkillID == NAME_None)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}
}

void UBTTask_PickBestAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UBTTask_PickBestAttack::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	return EBTNodeResult::Aborted;
}
