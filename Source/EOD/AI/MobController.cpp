// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MobController.h"
#include "EOD/Statics/AILibrary.h"

#include "BehaviorTree/BlackboardComponent.h"

AMobController::AMobController(const FObjectInitializer& ObjectInitializer)
{
	AggroActivationRadius = 500;
	AggroAreaRadius = 5000;
	MaxEnemyChaseRadius = 1000;
	WanderRadius = 3500;
}

void AMobController::InitializeBlackboardValues(UBlackboardComponent* BlackboardComponent)
{
	if (IsValid(BlackboardComponent))
	{
		if (GetPawn())
		{
			BlackboardComponent->SetValueAsVector(UAILibrary::BBKey_SpawnLocation, GetPawn()->GetActorLocation());
		}
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_AggroActivationRadius, AggroActivationRadius);
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_MaxEnemyChaseRadius, MaxEnemyChaseRadius);
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_AggroAreaRadius, AggroAreaRadius);
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_WanderRadius, WanderRadius);
	}
}
