// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTService_CheckForEnemies.h"
#include "AILibrary.h"
#include "CharacterLibrary.h"
#include "EODPreprocessors.h"
#include "EODCharacterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTService_CheckForEnemies::UBTService_CheckForEnemies(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBTService_CheckForEnemies::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTService_CheckForEnemies::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_CheckForEnemies::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	APawn* PawnOwner = IsValid(AIController) ? AIController->GetPawn() : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(PawnOwner) || !IsValid(BlackboardComp))
	{
		return;
	}

	UObject* EnemyObject = BlackboardComp->GetValueAsObject(UAILibrary::BBKey_TargetEnemy);
	if (IsValid(EnemyObject))
	{
		AEODCharacterBase* EnemyCharacter = Cast<AEODCharacterBase>(EnemyObject);
		if (IsValid(EnemyCharacter))
		{
			float MaxEnemyChaseRadius = BlackboardComp->GetValueAsFloat(UAILibrary::BBKey_MaxEnemyChaseRadius);
			float Distance = (PawnOwner->GetActorLocation() - EnemyCharacter->GetActorLocation()).Size();

			if (Distance < MaxEnemyChaseRadius)
			{
				FVector SpawnLocation = BlackboardComp->GetValueAsVector(UAILibrary::BBKey_SpawnLocation);
				FVector EnemyLocation = EnemyCharacter->GetActorLocation();

				float AggroAreaRadius = BlackboardComp->GetValueAsFloat(UAILibrary::BBKey_AggroAreaRadius);
				if ((SpawnLocation - EnemyLocation).Size() < AggroAreaRadius)
				{
					// Nothing to do. Continue chasing/attacking
					return;
				}
			}
		}
	}
	// If we don't have any valid target enemy set
	else
	{
		LookForAnotherEnemy(OwnerComp, NodeMemory, DeltaSeconds);
	}

	// @todo enemy detection logic needs improvement
}

void UBTService_CheckForEnemies::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UBTService_CheckForEnemies::LookForAnotherEnemy(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	UWorld* World = OwnerComp.GetWorld();
	if (!IsValid(BlackboardComp) || !IsValid(CharacterOwner) || !IsValid(World))
	{
		return;
	}

	BlackboardComp->SetValueAsObject(UAILibrary::BBKey_TargetEnemy, nullptr);
	float AggroActivationRadius = BlackboardComp->GetValueAsFloat(UAILibrary::BBKey_AggroActivationRadius);

	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AggroActivationRadius);
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(CharacterOwner);
	FVector OwnerLocation = CharacterOwner->GetActorLocation();

	FVector TraceEnd = OwnerLocation + FVector(0.f, 0.f, 1.f);
	bool bHit = World->SweepMultiByChannel(HitResults, OwnerLocation, TraceEnd, FQuat::Identity, COLLISION_COMBAT, CollisionShape, Params);
	for (FHitResult& HitResult : HitResults)
	{
		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitResult.GetActor());
		if (!IsValid(HitCharacter) || !UCharacterLibrary::AreEnemies(HitCharacter, CharacterOwner))
		{
			continue;
		}

		BlackboardComp->SetValueAsObject(UAILibrary::BBKey_TargetEnemy, HitCharacter);
		BlackboardComp->SetValueAsBool(UAILibrary::BBKey_bHasEnemyTarget, true);

		// Put character in combat state
		CharacterOwner->SetInCombat(true);
		return;
	}

	CharacterOwner->SetInCombat(false);
}
