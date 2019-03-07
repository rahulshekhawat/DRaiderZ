// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DodgeState.h"
#include "EODCharacterBase.h"
#include "Components/StatsComponentBase.h"

UDodgeState::UDodgeState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDodgeState::CanEnterState()
{
	if (IsValid(StateOwner))
	{
		UStatsComponentBase* StatsComp = StateOwner->GetCharacterStatsComponent();
		// if (StatsComp->)

	}

	return false;
}

void UDodgeState::OnEnterState()
{
}
