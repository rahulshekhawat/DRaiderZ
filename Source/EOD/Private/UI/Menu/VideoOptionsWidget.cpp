// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "VideoOptionsWidget.h"
#include "EODLibrary.h"
#include "ResolutionSubWidget.h"
#include "WindowModeSubWidget.h"
#include "ScrollButtonWidget.h"
#include "FrameRateSubWidget.h"
#include "QualitySettingSubWidget.h"
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
		WindowMode &&
		Resolution &&
		VerticalSync &&
		FrameRate &&
		GraphicsQuality &&
		Gamma &&
		WindowModeSub &&
		ResolutionSub &&
		FrameRateSub &&
		GraphicsSub &&
		SubWidgetSwitcher)
	{
		WindowMode->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleWindowModeButtonClicked);
		Resolution->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleResolutionButtonClicked);
		VerticalSync->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleVSyncButtonClicked);
		FrameRate->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleFrameRateButtonClicked);
		GraphicsQuality->OnClicked.AddDynamic(this, &UVideoOptionsWidget::HandleGraphicsQualityButtonClicked);

		WindowModeSub->OnWindowModeSelected.AddDynamic(this, &UVideoOptionsWidget::HandleWindowModeSelected);
		ResolutionSub->OnResolutionSelected.AddDynamic(this, &UVideoOptionsWidget::HandleResolutionSelected);
		FrameRateSub->OnFrameRateSelected.AddDynamic(this, &UVideoOptionsWidget::HandleFrameRateSelected);
		GraphicsSub->OnQualitySelected.AddDynamic(this, &UVideoOptionsWidget::HandleGraphicsQualitySelected);

		InitializeOptions(true);
		return true;
	}

	return false;
}

void UVideoOptionsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	check(WindowModeSub);
	check(ResolutionSub);
	check(FrameRateSub);

	WindowModeSub->SetParentOptionsWidget(this);
	ResolutionSub->SetParentOptionsWidget(this);
	FrameRateSub->SetParentOptionsWidget(this);

}

void UVideoOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeOptions();
}

void UVideoOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
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

void UVideoOptionsWidget::UpdateCurrentGraphicsQuality(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(GraphicsQuality);

	bool bCustom = false;
	int32 AAQL = GameUserSettings->GetAntiAliasingQuality();
	int32 VDQL = GameUserSettings->GetViewDistanceQuality();
	int32 VEQL = GameUserSettings->GetVisualEffectQuality();
	int32 ShadowQL = GameUserSettings->GetShadowQuality();
	int32 TQL = GameUserSettings->GetTextureQuality();
	int32 PPQL = GameUserSettings->GetPostProcessingQuality();
	int32 FQL = GameUserSettings->GetFoliageQuality();

	// If even one of the quality level is different from the rest
	if (AAQL != VDQL && AAQL != VEQL && AAQL != ShadowQL && AAQL != TQL && AAQL != PPQL && AAQL != FQL)
	{
		bCustom = true;
	}

	if (bCustom)
	{
		GraphicsQuality->SetInfoText(FText::FromString("CUSTOM"));
	}
	else
	{
		FText QLText;
		GetQualityText(AAQL, QLText);
		GraphicsQuality->SetInfoText(QLText);		
	}
}

void UVideoOptionsWidget::InitializeOptions(bool bForceUserSettingReload)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->LoadSettings(bForceUserSettingReload);
		UpdateCurrentWindowMode(GameUserSettings);
		UpdateCurrentResolution(GameUserSettings);
		UpdateCurrentVerticalSync(GameUserSettings);
		UpdateCurrentFrameRate(GameUserSettings);
		UpdateCurrentGraphicsQuality(GameUserSettings);
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

void UVideoOptionsWidget::HandleWindowModeButtonClicked()
{
	ToggleSubOptions(WindowModeSub, WindowMode);
}

void UVideoOptionsWidget::HandleResolutionButtonClicked()
{
	ToggleSubOptions(ResolutionSub, Resolution);
}

void UVideoOptionsWidget::HandleVSyncButtonClicked()
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	bool bVSyncChecked = VerticalSync->IsOptionChecked();
	if (GameUserSettings && GameUserSettings->IsVSyncEnabled() != bVSyncChecked)
	{
		GameUserSettings->SetVSyncEnabled(bVSyncChecked);
		bIsDirty = true;
	}
}

void UVideoOptionsWidget::HandleFrameRateButtonClicked()
{
	ToggleSubOptions(FrameRateSub, FrameRate);
}

void UVideoOptionsWidget::HandleGraphicsQualityButtonClicked()
{
	ToggleSubOptions(GraphicsSub, GraphicsQuality);
}

void UVideoOptionsWidget::HandleGraphicsQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings)
	{
		bIsDirty = true;
		GameUserSettings->SetAntiAliasingQuality(SelectedQuality);
		GameUserSettings->SetViewDistanceQuality(SelectedQuality);
		//~ @note For some reason changing visual effects quality freezes the game when saving game user settings
		GameUserSettings->SetVisualEffectQuality(SelectedQuality);
		GameUserSettings->SetShadowQuality(SelectedQuality);
		GameUserSettings->SetTextureQuality(SelectedQuality);
		GameUserSettings->SetPostProcessingQuality(SelectedQuality);
		GameUserSettings->SetFoliageQuality(SelectedQuality);
	}

	UpdateCurrentGraphicsQuality(GameUserSettings);

	ToggleSubOptions(GraphicsSub, GraphicsQuality);
}

void UVideoOptionsWidget::HandleWindowModeSelected(EWindowMode::Type SelectedWindowMode)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetFullscreenMode() != SelectedWindowMode)
	{
		bIsDirty = true;
		GameUserSettings->SetFullscreenMode(SelectedWindowMode);
	}
	UpdateCurrentWindowMode(GameUserSettings);

	ToggleSubOptions(WindowModeSub, WindowMode);
}

void UVideoOptionsWidget::HandleFrameRateSelected(float SelectedFrameRate)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetFrameRateLimit() != SelectedFrameRate)
	{
		bIsDirty = true;
		GameUserSettings->SetFrameRateLimit(SelectedFrameRate);
	}
	UpdateCurrentFrameRate(GameUserSettings);

	ToggleSubOptions(FrameRateSub, FrameRate);
}

void UVideoOptionsWidget::HandleResolutionSelected(FIntPoint SelectedResolution)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetScreenResolution() != SelectedResolution)
	{
		bIsDirty = true;
		GameUserSettings->SetScreenResolution(SelectedResolution);
	}
	UpdateCurrentResolution(GameUserSettings);

	ToggleSubOptions(ResolutionSub, Resolution);
}
