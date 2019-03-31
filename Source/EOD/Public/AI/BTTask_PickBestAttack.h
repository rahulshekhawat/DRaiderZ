// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PickBestAttack.generated.h"

/**
 * BTTask class to pick the best attack skill to use
 * 
 * @note Currently this class pics the best melee skill only.
 * It needs to be modified to pick a best attack skill of any kind (ranged or melee).
 * And the melee only class needs to be separate.
 */
UCLASS()
class EOD_API UBTTask_PickBestAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PickBestAttack(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
