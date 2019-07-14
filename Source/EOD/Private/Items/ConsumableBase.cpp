// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ConsumableBase.h"
#include "InventoryComponent.h"

UConsumableBase::UConsumableBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UConsumableBase::OnClick()
{
	bool bConsumeSuccess = Consume();
	if (bConsumeSuccess)
	{
		UInventoryComponent* InvComp = Cast<UInventoryComponent>(GetOuter());
		// InvComp->RemoveSingle


		// InvComp->
	}
}

bool UConsumableBase::Consume_Implementation()
{
	return false;
}
