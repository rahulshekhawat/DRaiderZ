// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODAIControllerBase.h"

#include "UnrealNetwork.h"

AEODAIControllerBase::AEODAIControllerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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
