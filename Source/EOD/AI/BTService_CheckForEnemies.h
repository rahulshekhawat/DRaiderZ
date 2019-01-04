// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForEnemies.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UBTService_CheckForEnemies : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckForEnemies(const FObjectInitializer& ObjectInitializer);

	/** called when auxiliary node becomes active
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** called when auxiliary node becomes inactive
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** tick function
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** called when search enters underlying branch
	 * this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

private:
	void LookForAnotherEnemy(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds);

};
