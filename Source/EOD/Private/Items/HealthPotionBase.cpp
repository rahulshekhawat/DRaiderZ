// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "HealthPotionBase.h"
#include "EODCharacterBase.h"

UHealthPotionBase::UHealthPotionBase(const FObjectInitializer& ObjectInitializer)
{
}

void UHealthPotionBase::OnClick_Implementation(AEODCharacterBase* ClickingChar) const
{
	if (IsValid(ClickingChar))
	{
		ClickingChar->Heal(Value);
	}
}
