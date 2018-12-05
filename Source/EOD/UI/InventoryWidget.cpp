// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InventoryWidget.h"

#include "Components/ScrollBox.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
}

bool UInventoryWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
