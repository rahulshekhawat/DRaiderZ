// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryInterface.h"

UInventoryInterface::UInventoryInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
}

void IInventoryInterface::OnClick_Implementation(AEODCharacterBase* ClickingChar) const
{
}

EInventoryItemType IInventoryInterface::GetInventoryItemType() const
{
	return EInventoryItemType::None;
}

FInGameInformation IInventoryInterface::GetInGameInformation() const
{
	return FInGameInformation();
}

FName IInventoryInterface::GetInventoryItemID() const
{
	return NAME_None;
}
