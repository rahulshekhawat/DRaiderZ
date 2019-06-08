// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MovementBuff.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"
#include "EOD.h"
#include "Components/AudioComponent.h"

#include "TimerManager.h"

UMovementBuff::UMovementBuff(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMovementBuff::ActivateEffect(int32 ActivationLevel)
{
	if (ActivationLevel < 1)
	{
		ActivationLevel = 1;
	}
	else if (ActivationLevel > MaxUpgradeLevel)
	{
		ActivationLevel = MaxUpgradeLevel;
	}

	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator)
	{
		bool bIsLocallyControlled = Instigator->Controller && Instigator->Controller->IsLocalController();
		if (bIsLocallyControlled)
		{
			Instigator->AddRunningModifier(this, true);
		}

		int32 NetDuration = GameplayEffectDuration + (ActivationLevel - 1) * (ExtraEffectDurationPerLevel);

		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(MovementEndTimerHandle, this, &UMovementBuff::DeactivateEffect, NetDuration, false);

		if (GameplaySound)
		{
			Instigator->GetGameplayAudioComponent()->SetSound(GameplaySound);
			Instigator->GetGameplayAudioComponent()->Play();
		}

		bActive = true;
	}
}

void UMovementBuff::DeactivateEffect()
{
	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator)
	{
		Instigator->RemoveRunningModifier(this);

		UWorld* World = Instigator->GetWorld();
		check(World);
		World->GetTimerManager().ClearTimer(MovementEndTimerHandle);

		bActive = false;

		UGameplaySkillsComponent* SkillsComp = InstigatorSkillComponent.Get();
		check(SkillsComp);
		SkillsComp->RemoveGameplayEffect(this);
	}
	else
	{
		this->MarkPendingKill();
	}
}

void UMovementBuff::UpdateEffect(float DeltaTime)
{
}
