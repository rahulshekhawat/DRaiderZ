// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "VideoOptionsWidget.h"

UVideoOptionsWidget::UVideoOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UVideoOptionsWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UVideoOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVideoOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVideoOptionsWidget::CloseDownOptions()
{
}
