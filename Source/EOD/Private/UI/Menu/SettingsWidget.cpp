// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SettingsWidget.h"

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USettingsWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USettingsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
