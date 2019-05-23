// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MovementBuff.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"

#include "TimerManager.h"

UMovementBuff::UMovementBuff(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMovementBuff::ActivateEffect()
{
	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator)
	{
		bool bIsLocallyControlled = Instigator->Controller && Instigator->Controller->IsLocalController();
		if (bIsLocallyControlled)
		{
			Instigator->SetIsRunning(true);
		}

		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(MovementEndTimerHandle, this, &UMovementBuff::DeactivateEffect, GameplayEffectDuration, false);

		bActive = true;
	}
}

void UMovementBuff::DeactivateEffect()
{
	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator)
	{
		UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
		check(SkillsComp);

		// If another gameplay effect of movement buff type is active, we do not need to disable running
		if (SkillsComp->IsGameplayEffectTypeActive(UMovementBuff::StaticClass(), this))
		{
		}
		else
		{
			if (Instigator->Controller)
			{
				Instigator->SetIsRunning(false);
			}
		}

		bActive = false;

		SkillsComp->RemoveGameplayEffect(this);

		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().ClearTimer(MovementEndTimerHandle);
	}
	else
	{
		this->MarkPendingKill();
	}
}

void UMovementBuff::UpdateEffect(float DeltaTime)
{
}
