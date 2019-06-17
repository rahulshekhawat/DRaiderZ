// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODAIControllerBase.h"
#include "AIStatsComponent.h"
#include "AILibrary.h"
#include "AICharacterBase.h"

#include "UnrealNetwork.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEODAIControllerBase::StatsComponentName(TEXT("AI Stats"));

AEODAIControllerBase::AEODAIControllerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StatsComponent = ObjectInitializer.CreateDefaultSubobject<UAIStatsComponent>(this, AEODAIControllerBase::StatsComponentName);

	AggroActivationRadius = 1000;
	AggroAreaRadius = 10000;
	MaxEnemyChaseRadius = 1500;
	WanderRadius = 5000;
}

void AEODAIControllerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AEODAIControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AEODAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEODAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODAIControllerBase::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	AICharacter = InPawn ? Cast<AAICharacterBase>(InPawn) : nullptr;

	if (StatsComponent && AICharacter)
	{
		int32 MaxValue = StatsComponent->Health.GetMaxValue();
		int32 CurrentValue = StatsComponent->Health.GetCurrentValue();

		AICharacter->UpdateHealth(MaxValue, CurrentValue);
		StatsComponent->Health.OnStatValueChanged.AddUObject(AICharacter, &AAICharacterBase::UpdateHealth);
	}
}

void AEODAIControllerBase::InitializeBlackboardValues(UBlackboardComponent* BlackboardComponent)
{
	if (IsValid(BlackboardComponent))
	{
		if (IsValid(GetPawn()))
		{
			BlackboardComponent->SetValueAsVector(UAILibrary::BBKey_SpawnLocation, GetPawn()->GetActorLocation());
		}
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_AggroActivationRadius, AggroActivationRadius);
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_MaxEnemyChaseRadius, MaxEnemyChaseRadius);
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_AggroAreaRadius, AggroAreaRadius);
		BlackboardComponent->SetValueAsFloat(UAILibrary::BBKey_WanderRadius, WanderRadius);
	}
}
