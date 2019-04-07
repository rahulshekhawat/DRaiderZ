// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODAIControllerBase.h"
#include "AIStatsComponent.h"

#include "UnrealNetwork.h"

const FName AEODAIControllerBase::StatsComponentName(TEXT("AI Stats"));

AEODAIControllerBase::AEODAIControllerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StatsComponent = ObjectInitializer.CreateDefaultSubobject<UAIStatsComponent>(this, AEODAIControllerBase::StatsComponentName);
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
