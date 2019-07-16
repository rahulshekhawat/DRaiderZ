// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MovementBuff.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"
#include "EOD.h"
#include "Components/AudioComponent.h"

#include "TimerManager.h"

UMovementBuff::UMovementBuff(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNeedsUpdate = false;
}

void UMovementBuff::InitEffect(AEODCharacterBase* Instigator, TArray<AEODCharacterBase*> Targets, int32 ActivationLevel)
{
	Super::InitEffect(Instigator, Targets, ActivationLevel);

	if (ActivationLevel > MaxUpgradeLevel)
	{
		CurrentLevel = MaxUpgradeLevel;
	}
}

void UMovementBuff::ActivateEffect_Implementation()
{
	AEODCharacterBase* Instigator = EffectInstigator.Get();
	if (Instigator)
	{
		bool bIsLocallyControlled = Instigator->Controller && Instigator->Controller->IsLocalController();
		if (bIsLocallyControlled)
		{
			Instigator->AddRunningModifier(this, true);
		}

		int32 NetDuration = GetDuration();

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

void UMovementBuff::DeactivateEffect_Implementation()
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

void UMovementBuff::UpdateEffect_Implementation(float DeltaTime)
{
}

float UMovementBuff::GetDuration() const
{
	return (BaseDuration + (CurrentLevel - 1) * (ExtraEffectDurationPerLevel));
}
