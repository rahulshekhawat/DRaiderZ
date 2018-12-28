// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PauseMenuWidget.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UPauseMenuWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
