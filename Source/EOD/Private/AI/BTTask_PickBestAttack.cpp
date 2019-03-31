// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_PickBestAttack.h"
#include "EODCharacterBase.h"
#include "AILibrary.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTask_PickBestAttack::UBTTask_PickBestAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NodeName = "Pick Best Attack";
    bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_PickBestAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(CharacterOwner) || !IsValid(BlackboardComp))
	{
		return EBTNodeResult::Failed;
	}

	AEODCharacterBase* TargetCharacter = Cast<AEODCharacterBase>(BlackboardComp->GetValueAsObject(UAILibrary::BBKey_TargetEnemy));

	FName MostWeightedSkillID = CharacterOwner->GetMostWeightedMeleeSkillID(TargetCharacter);
	BlackboardComp->SetValueAsName(UAILibrary::BBKey_MostWeightedSkillID, MostWeightedSkillID);
	if (MostWeightedSkillID == NAME_None)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}
}

void UBTTask_PickBestAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UBTTask_PickBestAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
