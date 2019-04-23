// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatManager.h"
#include "EODPreprocessors.h"
#include "PlayerCharacter.h"
#include "AttackDodgedEvent.h"
#include "StatsComponentBase.h"
#include "GameplaySkillsComponent.h"
#include "EODAIControllerBase.h"
#include "EODPlayerController.h"

#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

ACombatManager::ACombatManager(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(false);
	SetReplicateMovement(false);
	PhysicalCritMultiplier = 1.6f;
	MagickalCritMultiplier = 1.4f;
}

void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatManager::OnMeleeAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	ICombatInterface* InstigatorCI = Cast<ICombatInterface>(HitInstigator);
	if (!HitInstigator || !InstigatorCI)
	{
		return;
	}

	TArray<FAttackResponse> AttackResponses;
	TArray<AActor*> HitActors;
	if (HitResults.Num() == 0)
	{
		InstigatorCI->PostAttack(AttackResponses, HitActors);
		return;
	}

	TSharedPtr<FAttackInfo> AttackInfoPtr = InstigatorCI->GetAttackInfoPtr();
	TArray<TSharedPtr<FAttackResponse>> AttackResponsePtrs;
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		ICombatInterface* TargetCI = Cast<ICombatInterface>(HitActor);

		// Do not process if the hit actor does not implement a combat interface
		if (!TargetCI)
		{
			continue;
		}

		TSharedPtr<FAttackResponse> AttackResponsePtr = ProcessAttack(HitInstigator, InstigatorCI, AttackInfoPtr, HitActor, TargetCI, HitResult);
		if (AttackResponsePtr.IsValid())
		{
			AttackResponsePtrs.Add(AttackResponsePtr);
			HitActors.Add(HitActor);
			FAttackResponse* ResponsePtr = AttackResponsePtr.Get();
			if (ResponsePtr)
			{
				AttackResponses.Add(*ResponsePtr);
			}
		}
	}

	InstigatorCI->PostAttack(AttackResponses, HitActors);
}

TSharedPtr<FAttackResponse> ACombatManager::ProcessAttack(AActor* HitInstigator, ICombatInterface* InstigatorCI, const TSharedPtr<FAttackInfo>& AttackInfoPtr, AActor* HitTarget, ICombatInterface* TargetCI, const FHitResult& HitResult)
{
	check(InstigatorCI && TargetCI);

	TSharedPtr<FAttackResponse> DamageResponsePtr(nullptr);
	if (!AttackInfoPtr.IsValid() || !InstigatorCI->IsEnemyOf(TargetCI))
	{
		return DamageResponsePtr;
	}

	FHitResult LineHitResult;
	bool bLineHitResultFound;
	GetLineHitResult(HitInstigator, HitResult.GetComponent(), LineHitResult, bLineHitResultFound);

	DamageResponsePtr = TargetCI->ReceiveAttack(HitInstigator, InstigatorCI, AttackInfoPtr, HitResult, bLineHitResultFound, LineHitResult);

	return DamageResponsePtr;
}

void ACombatManager::GetLineHitResult(const AActor* HitInstigator, const AActor* HitTarget, FHitResult& OutHitResult, bool& bOutLineHitResultFound) const
{
	bOutLineHitResultFound = false;
	if (!IsValid(HitInstigator) || !IsValid(HitTarget))
	{
		return;
	}

	FCollisionQueryParams QueryParams = UCombatLibrary::GenerateCombatCollisionQueryParams(HitInstigator);
	FVector LineStart = HitInstigator->GetActorLocation();
	FVector LineEnd = HitTarget->GetActorLocation();
	LineEnd.Z = LineStart.Z < LineEnd.Z ? LineStart.Z : LineEnd.Z;

	TArray<FHitResult> LineHitResults;
	GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, QueryParams);
	for (FHitResult& LineHitResult : LineHitResults)
	{
		if (LineHitResult.GetActor() == HitTarget)
		{
			OutHitResult = LineHitResult;
			bOutLineHitResultFound = true;
			break;
		}
	}
}

void ACombatManager::GetLineHitResult(const AActor* HitInstigator, const UPrimitiveComponent* HitComponent, FHitResult& OutHitResult, bool& bOutLineHitResultFound) const
{
	bOutLineHitResultFound = false;
	if (!IsValid(HitInstigator) || !IsValid(HitComponent))
	{
		return;
	}

	FCollisionQueryParams QueryParams = UCombatLibrary::GenerateCombatCollisionQueryParams(HitInstigator);
	FVector LineStart = HitInstigator->GetActorLocation();
	FVector LineEnd = HitComponent->GetComponentLocation();
	LineEnd.Z = LineStart.Z < LineEnd.Z ? LineStart.Z : LineEnd.Z;

	TArray<FHitResult> LineHitResults;
	GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, QueryParams);
	for (FHitResult& LineHitResult : LineHitResults)
	{
		if (LineHitResult.GetComponent() == HitComponent)
		{
			OutHitResult = LineHitResult;
			bOutLineHitResultFound = true;
			break;
		}
	}
}

bool ACombatManager::AreEnemies(AEODCharacterBase* CharOne, AEODCharacterBase* CharTwo)
{
	ICombatInterface* CIOne = Cast<ICombatInterface>(CharOne);
	ICombatInterface* CITwo = Cast<ICombatInterface>(CharTwo);

	bool bResult = false;
	if (CIOne && CITwo)
	{
		bResult = CIOne->IsEnemyOf(CITwo);
	}
	return bResult;
}
