// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "FrameRateSubWidget.h"

UFrameRateSubWidget::UFrameRateSubWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UFrameRateSubWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UFrameRateSubWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFrameRateSubWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFrameRateSubWidget::SetParentOptionsWidget(UVideoOptionsWidget* NewParent)
{
	ParentOptionsWidget = NewParent;
}
