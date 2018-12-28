// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusEffectsManager.h"

AStatusEffectsManager::AStatusEffectsManager(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void AStatusEffectsManager::BeginPlay()
{
    Super::BeginPlay();

	// StatusEffectActivated.Broadcast()

}

void AStatusEffectsManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
