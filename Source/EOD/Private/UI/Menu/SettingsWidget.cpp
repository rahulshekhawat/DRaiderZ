// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SettingsWidget.h"

#include "Components/WidgetSwitcher.h"

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USettingsWidget::Initialize()
{
	if (Super::Initialize() &&
		OptionsWidgetSwitcher)
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

void USettingsWidget::SetWidgetSwitcherLayout(UWidget* NewActiveWidget, URegularButtonWidget* InCallingButton)
{
	UWidget* CurrentActiveWidget = OptionsWidgetSwitcher->GetActiveWidget();
	if (NewActiveWidget != CurrentActiveWidget || CurrentActiveWidget->GetVisibility() != ESlateVisibility::Visible)
	{

	}

}
