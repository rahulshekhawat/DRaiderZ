// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "HealthPotionBase.h"
#include "EODCharacterBase.h"

UHealthPotionBase::UHealthPotionBase(const FObjectInitializer& ObjectInitializer)
{
	PotionType = EInventoryItemType::Consumable;
}

void UHealthPotionBase::OnClick_Implementation(AEODCharacterBase* ClickingChar) const
{
	if (IsValid(ClickingChar) == false)
	{
		return;
	}




	// if (PotionDrinkingAnimation)
	{
	}
	// else
	{
		ClickingChar->Heal(HealthRegenValue);
	}
}

EInventoryItemType UHealthPotionBase::GetInventoryItemType() const
{
	return PotionType;
}

FInGameInformation UHealthPotionBase::GetInGameInformation() const
{
	return PotionInformation;
}
