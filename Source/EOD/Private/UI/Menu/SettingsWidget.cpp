// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SettingsWidget.h"
#include "EODPreprocessors.h"
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
		OptionsWidgetSwitcher &&
		GameButton &&
		ControllerButton &&
		MouseKeyboardButton &&
		AudioButton &&
		VideoButton &&
		AdvancedButton)
	{
		GameButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleGameButtonClicked);
		ControllerButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleControllerButtonClicked);
		MouseKeyboardButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleMouseKeyButtonClicked);
		AudioButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleAudioButtonClicked);
		VideoButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleVideoButtonClicked);
		AdvancedButton->OnClicked.AddDynamic(this, &USettingsWidget::HandleAdvancedButtonClicked);

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

		UnlockAllButtons();

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
		}
	}
}

void USettingsWidget::HandleGameButtonClicked()
{
	//~ @todo
	OptionsNameTextBlock->SetText(FText::FromString("GAME"));
}

void USettingsWidget::HandleControllerButtonClicked()
{
	//~ @todo
	OptionsNameTextBlock->SetText(FText::FromString("CONTROLLER"));
}

void USettingsWidget::HandleMouseKeyButtonClicked()
{
	//~ @todo
	OptionsNameTextBlock->SetText(FText::FromString("MOUSE & KEYBOARD"));
}

void USettingsWidget::HandleAudioButtonClicked()
{
	check(OptionsNameTextBlock);
	SetWidgetSwitcherLayout(AudioOptionsMain, AudioButton);
	OptionsNameTextBlock->SetText(FText::FromString("AUDIO"));
}

void USettingsWidget::HandleVideoButtonClicked()
{
	check(OptionsNameTextBlock);
	SetWidgetSwitcherLayout(VideoOptionsMain, VideoButton);
	OptionsNameTextBlock->SetText(FText::FromString("VIDEO"));
}

void USettingsWidget::HandleAdvancedButtonClicked()
{
	check(OptionsNameTextBlock);
	SetWidgetSwitcherLayout(AdvancedOptionsMain, AdvancedButton);
	OptionsNameTextBlock->SetText(FText::FromString("ADVANCED"));
}
