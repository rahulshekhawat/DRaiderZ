// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "VideoOptionsWidget.h"
#include "EODLibrary.h"
#include "ResolutionWidget.h"
#include "ScrollButtonWidget.h"
#include "MainMenuPlayerController.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"

UVideoOptionsWidget::UVideoOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UVideoOptionsWidget::Initialize()
{
	if (Super::Initialize() &&
		Resolution &&
		VerticalSync)
	{
		Resolution->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleResolutionButtonClicked);
		VerticalSync->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleVSyncButtonClicked);

		return true;
	}

	return false;
}

void UVideoOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bIsInitialized)
	{
		InitializeOptions();
	}
}

void UVideoOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UVideoOptionsWidget::EnableScrollBoxItems()
{
	if (VideoOptionsScrollBox)
	{
		int32 ChildCount = VideoOptionsScrollBox->GetChildrenCount();
		for (int i = 0; i < ChildCount; i++)
		{
			UWidget* Widget = VideoOptionsScrollBox->GetChildAt(i);
			if (Widget)
			{
				Widget->SetIsEnabled(true);
			}
		}
	}
}

void UVideoOptionsWidget::DisableScrollBoxItems(UWidget* ExcludedItem)
{
	if (VideoOptionsScrollBox)
	{
		int32 ChildCount = VideoOptionsScrollBox->GetChildrenCount();
		for (int i = 0; i < ChildCount; i++)
		{
			UWidget* Widget = VideoOptionsScrollBox->GetChildAt(i);
			if (Widget && Widget != ExcludedItem)
			{
				Widget->SetIsEnabled(false);
			}
		}
	}
}

void UVideoOptionsWidget::MoveSubMenu(UWidget* InScrollItem)
{
	// SubWidgetSwitcher
}

void UVideoOptionsWidget::ToggleSubOptions(UWidget* SubWidget, UScrollButtonWidget* CallingScrollButton)
{
	check(SubWidgetSwitcher);

	if (SubWidgetSwitcher->GetVisibility() == ESlateVisibility::Visible)
	{
		EnableScrollBoxItems();
		SubWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SubWidgetSwitcher->SetActiveWidget(SubWidget);
		SubWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		DisableScrollBoxItems(CallingScrollButton);
		MoveSubMenu(CallingScrollButton);
	}
}

void UVideoOptionsWidget::CloseDownOptions()
{
	EnableScrollBoxItems();
	SubWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);

	if (!bIsDirty)
	{
		return;
	}

	bIsDirty = false;

	//~ @todo display action confirmation widget
	/*
	AMainMenuPlayerController* PC = Cast<AMainMenuPlayerController>(GetOwningPlayer());
	if (PC)
	{
	}
	else
	{

	}
	*/
}

void UVideoOptionsWidget::InitializeOptions()
{
	bIsInitialized = true;

	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->LoadSettings(true);
		UpdateCurrentWindowMode(GameUserSettings);
		UpdateCurrentResolution(GameUserSettings);
		UpdateCurrentVerticalSync(GameUserSettings);
		UpdateCurrentFrameRate(GameUserSettings);
	}
}

void UVideoOptionsWidget::ResetOptions()
{
	InitializeOptions();
}

void UVideoOptionsWidget::UpdateCurrentWindowMode(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(WindowMode);

	EWindowMode::Type FullScreenMode = GameUserSettings->GetFullscreenMode();
	switch (FullScreenMode)
	{
	case EWindowMode::Fullscreen:
		WindowMode->SetInfoText(FText::FromString("FULLSCREEN"));
		break;
	case EWindowMode::WindowedFullscreen:
		WindowMode->SetInfoText(FText::FromString("BORDERLESS"));
		break;
	case EWindowMode::Windowed:
		WindowMode->SetInfoText(FText::FromString("WINDOW"));
		break;
	default:
		break;
	}
}

void UVideoOptionsWidget::UpdateCurrentResolution(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(Resolution);

	FIntPoint CurrentResolution = GameUserSettings->GetScreenResolution();
	FString ResolutionString = FString::FromInt(CurrentResolution.X) + FString("x") + FString::FromInt(CurrentResolution.Y);
	Resolution->SetInfoText(FText::FromString(ResolutionString));
}

void UVideoOptionsWidget::UpdateCurrentVerticalSync(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(VerticalSync);

	bool bIsVSyncEnabled = GameUserSettings->IsVSyncEnabled();
	VerticalSync->SetChecked(bIsVSyncEnabled);
}

void UVideoOptionsWidget::UpdateCurrentFrameRate(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(FrameRate);

	float FrameRateFloat = GameUserSettings->GetFrameRateLimit();
	int32 FrameRateInt = FMath::TruncToInt(FrameRateFloat);
	if (FrameRateInt > 0)
	{
		FrameRate->SetInfoText(FText::AsNumber(FrameRateInt));
	}
	else
	{
		FrameRate->SetInfoText(FText::FromString("UNLIMITED"));
	}

}

void UVideoOptionsWidget::HandleResolutionButtonClicked()
{
	if (ResolutionSub)
	{
		ResolutionSub->SetParentWidget(this);
		ToggleSubOptions(ResolutionSub, Resolution);
	}
}

void UVideoOptionsWidget::HandleVSyncButtonClicked()
{
	//~ @todo fix
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	bool bVSyncChecked = VerticalSync->IsOptionChecked();
	if (GameUserSettings && GameUserSettings->IsVSyncEnabled() != bVSyncChecked)
	{
		GameUserSettings->SetVSyncEnabled(bVSyncChecked);
		bIsDirty = true;
	}


	// VerticalSync->IsOptionChecked()
}
