// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MainMenuWidget.h"

#include "Components/Button.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UMainMenuWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
