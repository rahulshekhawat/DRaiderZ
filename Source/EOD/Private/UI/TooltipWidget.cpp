// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "TooltipWidget.h"

UTooltipWidget::UTooltipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UTooltipWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTooltipWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
