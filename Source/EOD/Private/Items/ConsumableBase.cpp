// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ConsumableBase.h"
#include "InventoryComponent.h"
#include "EODPlayerController.h"

UConsumableBase::UConsumableBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UConsumableBase::OnClick(AEODPlayerController* InPC)
{
	bool bConsumeSuccess = Consume(InPC);
	if (bConsumeSuccess)
	{
		UInventoryComponent* InvComp = Cast<UInventoryComponent>(GetOuter());
		// InvComp->RemoveSingle


		// InvComp->
	}
}

bool UConsumableBase::Consume_Implementation(AEODPlayerController* InPC)
{
	return false;
}
