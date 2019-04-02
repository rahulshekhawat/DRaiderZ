// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "FuriousDashBase.h"
#include "PlayerCharacter.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

UFuriousDashBase::UFuriousDashBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	StackLimit = 1;
	ActivationChance = 1.f;
	bTriggersOnUsingSkill = true;

	StatusEffectDuration = TickInterval = 4.f;
}

void UFuriousDashBase::OnStatusEffectTick_Implementation(AEODCharacterBase* TargetCharacter)
{
	if (IsValid(TargetCharacter))
	{
		APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(TargetCharacter);
		if (!PlayerChar)
		{
			DeactivateStatusEffect(TargetCharacter);
			return;
		}

		FStatusInfo& StatusInfo = CharacterToStatusInfoMap[TargetCharacter];
		if (StatusInfo.TotalElapsedTime == 0)
		{
			// ~ @todo
			// PlayerChar->EnableFastRun();
		}
		else if (StatusInfo.TotalElapsedTime >= StatusEffectDuration)
		{
			//~ @todo
			// PlayerChar->DisableFastRun();
			DeactivateStatusEffect(TargetCharacter);
			return;
		}

		StatusInfo.TotalElapsedTime += GetWorld()->GetTimerManager().GetTimerElapsed(*StatusInfo.TimerHandlePtr.Get());
	}
}
