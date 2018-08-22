// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTService_CheckForEnemies.h"
#include "Player/EODCharacterBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UBTService_CheckForEnemies::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_CheckForEnemies::OnCeaseRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_CheckForEnemies::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AActor* Owner = OwnerComp.GetOwner();
	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(Owner);

	bool bAggroed = OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("bAggroed"));

	if (bAggroed)
	{

	}
	else
	{
		// float AggroActivationRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("AggroActivationRadius"));
		// FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(AggroActivationRadius);

		// FCollisionShape Sphere = FCollisionShape::MakeCapsule(, HalfHeightVector.Size());
		// TArray<FHitResult> HitResults;
		
		// OwnerComp.GetWorld()->SweepMultiByChannel()
	}
}

void UBTService_CheckForEnemies::OnSearchStart(FBehaviorTreeSearchData & SearchData)
{
	Super::OnSearchStart(SearchData);
}
