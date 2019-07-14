// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ConsumableBase.h"

UConsumableBase::UConsumableBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UConsumableBase::OnClick()
{
	Consume();
}

void UConsumableBase::Consume_Implementation()
{
}
