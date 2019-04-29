// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "WindowModeSubWidget.h"

UWindowModeSubWidget::UWindowModeSubWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UWindowModeSubWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UWindowModeSubWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWindowModeSubWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
