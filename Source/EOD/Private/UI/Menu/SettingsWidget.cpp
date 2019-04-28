// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SettingsWidget.h"
#include "OptionsWidgetBase.h"
#include "RegularButtonWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
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
	if (CurrentActiveWidget && NewActiveWidget != CurrentActiveWidget || CurrentActiveWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		UOptionsWidgetBase* CurrentOptions = Cast<UOptionsWidgetBase>(CurrentActiveWidget);
		if (CurrentOptions)
		{
			CurrentOptions->CloseDownOptions();
		}

		OptionsWidgetSwitcher->SetActiveWidget(NewActiveWidget);
		OptionsWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		OptionsNameTextBlock->SetVisibility(ESlateVisibility::Visible);

		if (InCallingButton)
		{
			InCallingButton->LockButton(true);
		}
	}
}

void USettingsWidget::UnlockAllButtons()
{
	if (MainCanvasPanel == nullptr)
	{
		return;
	}

	int32 ChildrenCount = MainCanvasPanel->GetChildrenCount();
	for (int i = 0; i < ChildrenCount; i++)
	{
		URegularButtonWidget* RegularButton = Cast<URegularButtonWidget>(MainCanvasPanel->GetChildAt(i));
		if (RegularButton)
		{
			RegularButton->LockButton(false);
			RegularButton->StartUnhovered();
		}
	}
}
