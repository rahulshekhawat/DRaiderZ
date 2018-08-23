// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTService_CheckForEnemies.h"
#include "Player/EODCharacterBase.h"
#include "Core/EODPreprocessors.h"
#include "Statics/CharacterLibrary.h"
#include "Statics/AILibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTService_CheckForEnemies::UBTService_CheckForEnemies(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBTService_CheckForEnemies::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_CheckForEnemies::OnCeaseRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_CheckForEnemies::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AActor* Owner = OwnerComp.GetOwner();
	AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(Owner);

	bool bHasEnemyTarget = OwnerComp.GetBlackboardComponent()->GetValueAsBool(UAILibrary::BBKey_bHasEnemyTarget);

	if (bHasEnemyTarget)
	{
		AEODCharacterBase* EnemyCharacter = Cast<AEODCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(UAILibrary::BBKey_TargetEnemy));
		
		if (EnemyCharacter)
		{
			float MaxEnemyChaseRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(UAILibrary::BBKey_MaxEnemyChaseRadius);
			float Distance = (Owner->GetActorLocation() - EnemyCharacter->GetActorLocation()).Size();

			if (Distance < MaxEnemyChaseRadius)
			{
				FVector SpawnLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(UAILibrary::BBKey_SpawnLocation);
				FVector EnemyLocation = EnemyCharacter->GetActorLocation();

				float AggroAreaRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(UAILibrary::BBKey_AggroAreaRadius);

				if ((SpawnLocation - EnemyLocation).Size() < AggroAreaRadius)
				{
					// Nothing to do. Continue chasing/attacking
					return;
				}
			}
		}
	}

	LookForAnotherEnemy(OwnerComp, NodeMemory, DeltaSeconds);

}

void UBTService_CheckForEnemies::OnSearchStart(FBehaviorTreeSearchData & SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UBTService_CheckForEnemies::LookForAnotherEnemy(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(UAILibrary::BBKey_TargetEnemy, nullptr);

	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(OwnerComp.GetOwner());
	float AggroActivationRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("AggroActivationRadius"));

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AggroActivationRadius);
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(OwnerComp.GetOwner());
	FVector OwnerLocation = OwnerComp.GetOwner()->GetActorLocation();

	bool bHit = OwnerComp.GetWorld()->SweepMultiByChannel(HitResults, OwnerLocation, OwnerLocation, FQuat::Identity, COLLISION_COMBAT, CollisionShape, Params);

	for (FHitResult& HitResult : HitResults)
	{
		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitResult.GetActor());
		if (!HitCharacter )
		{
			continue;
		}

		if (!UCharacterLibrary::AreEnemies(HitCharacter, OwningCharacter))
		{
			continue;
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(UAILibrary::BBKey_TargetEnemy, HitCharacter);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(UAILibrary::BBKey_bHasEnemyTarget, true);
		break;
	}
}
