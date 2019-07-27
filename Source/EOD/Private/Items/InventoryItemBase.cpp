// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "InventoryItemBase.h"

UInventoryItemBase::UInventoryItemBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bStackable = true;
}

void UInventoryItemBase::OnClick(AEODPlayerController* InPC)
{
}
