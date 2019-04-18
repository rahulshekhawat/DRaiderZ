// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "BTTask_SelectBestMeleeAttack.h"
#include "EODAIControllerBase.h"
#include "EODCharacterBase.h"
#include "AILibrary.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectBestMeleeAttack::UBTTask_SelectBestMeleeAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Select Best Melee Attack";
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_SelectBestMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(CharacterOwner) || !IsValid(BlackboardComp))
	{
		return EBTNodeResult::Failed;
	}

	AEODCharacterBase* TargetEnemy = Cast<AEODCharacterBase>(BlackboardComp->GetValueAsObject(UAILibrary::BBKey_TargetEnemy));
	FName SkillID = CharacterOwner->GetMostWeightedMeleeSkillID(TargetEnemy);

	BlackboardComp->SetValueAsName(UAILibrary::BBKey_MostWeightedSkillID, SkillID);

	if (SkillID == NAME_None)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}
}

void UBTTask_SelectBestMeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UBTTask_SelectBestMeleeAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
