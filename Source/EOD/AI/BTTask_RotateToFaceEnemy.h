// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceEnemy.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UBTTask_RotateToFaceEnemy : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_RotateToFaceEnemy(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	/** Success condition precision in degrees */
	UPROPERTY(EditAnywhere, Category = Node)
	float Precision;

};
