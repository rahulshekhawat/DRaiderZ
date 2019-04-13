// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MovementBuff.h"
#include "EODCharacterBase.h"

#include "TimerManager.h"

UMovementBuff::UMovementBuff(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMovementBuff::ActivateEffect()
{
	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator && Instigator->Controller && Instigator->Controller->IsLocalPlayerController())
	{
		Instigator->SetIsRunning(true);
		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(MovementEndTimerHandle, this, &UMovementBuff::DeactivateEffect, GameplayEffectDuration, false);
	}
}

void UMovementBuff::DeactivateEffect()
{
	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator && Instigator->Controller)
	{
		Instigator->SetIsRunning(false);
	}
}

void UMovementBuff::UpdateEffect(float DeltaTime)
{
}
