// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "NonPlayerActorBase.h"

ANonPlayerActorBase::ANonPlayerActorBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANonPlayerActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANonPlayerActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

