// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AdvancedOptionsWidget.h"
#include "EODLibrary.h"
#include "ScrollButtonWidget.h"
#include "QualitySettingSubWidget.h"

#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"

UAdvancedOptionsWidget::UAdvancedOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAdvancedOptionsWidget::Initialize()
{
	if (Super::Initialize() &&
		AntiAliasing &&
		ViewDistance &&
		Effects &&
		Lighting &&
		Textures &&
		PostProcessing &&
		Foliage &&
		ResolutionScale &&
		AntiAliasingSub &&
		ViewDistanceSub &&
		EffectsSub &&
		LightingSub &&
		TexturesSub &&
		PostProcessingSub &&
		FoliageSub)
	{
		AntiAliasing->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandleAntiAliasingButtonClicked);
		ViewDistance->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandleViewDistanceButtonClicked);
		Effects->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandleEffectsButtonClicked);
		Lighting->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandleLightingButtonClicked);
		Textures->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandleTexturesButtonClicked);
		PostProcessing->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandlePostProcessingButtonClicked);
		Foliage->OnClicked.AddDynamic(this, &UAdvancedOptionsWidget::HandleFoliageButtonClicked);

		AntiAliasingSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandleAntiAliasingQualitySelected);
		ViewDistanceSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandleViewDistanceQualitySelected);
		EffectsSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandleEffectsQualitySelected);
		LightingSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandleLightingQualitySelected);
		TexturesSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandleTexturesQualitySelected);
		PostProcessingSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandlePostProcessingQualitySelected);
		FoliageSub->OnQualitySelected.AddDynamic(this, &UAdvancedOptionsWidget::HandleFoliageQualitySelected);

		ResolutionScale->OnSliderValueChanged.AddDynamic(this, &UAdvancedOptionsWidget::HandleResolutionScaleSliderValueChanged);

		InitializeOptions(true);
		return true;
	}

	return false;
}

void UAdvancedOptionsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	check(AntiAliasingSub);
	check(ViewDistanceSub);
	check(EffectsSub);
	check(LightingSub);
	check(TexturesSub);
	check(PostProcessingSub);
	check(FoliageSub);

	AntiAliasingSub->SetCustomParentWidget(this);
	ViewDistanceSub->SetCustomParentWidget(this);
	EffectsSub->SetCustomParentWidget(this);
	LightingSub->SetCustomParentWidget(this);
	TexturesSub->SetCustomParentWidget(this);
	PostProcessingSub->SetCustomParentWidget(this);
	FoliageSub->SetCustomParentWidget(this);
}

void UAdvancedOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeOptions();
}

void UAdvancedOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UAdvancedOptionsWidget::InitializeOptions(bool bForceUserSettingReload)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->LoadSettings(bForceUserSettingReload);
		UpdateCurrentAntiAliasing(GameUserSettings);
		UpdateCurrentViewDistance(GameUserSettings);
		UpdateCurrentEffects(GameUserSettings);
		UpdateCurrentLighting(GameUserSettings);
		UpdateCurrentTextures(GameUserSettings);
		UpdateCurrentPostProcessing(GameUserSettings);
		UpdateCurrentFoliage(GameUserSettings);
	}
}

void UAdvancedOptionsWidget::UpdateCurrentAntiAliasing(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(AntiAliasing);

	int32 Quality = GameUserSettings->GetAntiAliasingQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	AntiAliasing->SetInfoText(Text);
}

void UAdvancedOptionsWidget::UpdateCurrentViewDistance(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(ViewDistance);

	int32 Quality = GameUserSettings->GetViewDistanceQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	ViewDistance->SetInfoText(Text);
}

void UAdvancedOptionsWidget::UpdateCurrentEffects(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(Effects);

	int32 Quality = GameUserSettings->GetVisualEffectQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	Effects->SetInfoText(Text);
}

void UAdvancedOptionsWidget::UpdateCurrentLighting(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(Lighting);

	int32 Quality = GameUserSettings->GetShadowQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	Lighting->SetInfoText(Text);
}

void UAdvancedOptionsWidget::UpdateCurrentTextures(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(Textures);

	int32 Quality = GameUserSettings->GetTextureQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	Textures->SetInfoText(Text);
}

void UAdvancedOptionsWidget::UpdateCurrentPostProcessing(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(PostProcessing);

	int32 Quality = GameUserSettings->GetPostProcessingQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	PostProcessing->SetInfoText(Text);
}

void UAdvancedOptionsWidget::UpdateCurrentFoliage(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(Foliage);

	int32 Quality = GameUserSettings->GetFoliageQuality();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	Foliage->SetInfoText(Text);
}

void UAdvancedOptionsWidget::HandleAntiAliasingButtonClicked()
{
	ToggleSubOptions(AntiAliasingSub, AntiAliasing);
}

void UAdvancedOptionsWidget::HandleViewDistanceButtonClicked()
{
	ToggleSubOptions(ViewDistanceSub, ViewDistance);
}

void UAdvancedOptionsWidget::HandleEffectsButtonClicked()
{
	ToggleSubOptions(EffectsSub, Effects);
}

void UAdvancedOptionsWidget::HandleLightingButtonClicked()
{
	ToggleSubOptions(LightingSub, Lighting);
}

void UAdvancedOptionsWidget::HandleTexturesButtonClicked()
{
	ToggleSubOptions(TexturesSub, Textures);
}

void UAdvancedOptionsWidget::HandlePostProcessingButtonClicked()
{
	ToggleSubOptions(PostProcessingSub, PostProcessing);
}

void UAdvancedOptionsWidget::HandleFoliageButtonClicked()
{
	ToggleSubOptions(FoliageSub, Foliage);
}

void UAdvancedOptionsWidget::HandleAntiAliasingQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetAntiAliasingQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetAntiAliasingQuality(SelectedQuality);
	}
	UpdateCurrentAntiAliasing(GameUserSettings);

	ToggleSubOptions(AntiAliasingSub, AntiAliasing);
}

void UAdvancedOptionsWidget::HandleViewDistanceQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetViewDistanceQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetViewDistanceQuality(SelectedQuality);
	}
	UpdateCurrentViewDistance(GameUserSettings);

	ToggleSubOptions(ViewDistanceSub, ViewDistance);
}

void UAdvancedOptionsWidget::HandleEffectsQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetVisualEffectQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetVisualEffectQuality(SelectedQuality);
	}
	UpdateCurrentEffects(GameUserSettings);

	ToggleSubOptions(EffectsSub, Effects);
}

void UAdvancedOptionsWidget::HandleLightingQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetShadowQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetShadowQuality(SelectedQuality);
	}
	UpdateCurrentLighting(GameUserSettings);

	ToggleSubOptions(LightingSub, Lighting);
}

void UAdvancedOptionsWidget::HandleTexturesQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetTextureQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetTextureQuality(SelectedQuality);
	}
	UpdateCurrentTextures(GameUserSettings);

	ToggleSubOptions(TexturesSub, Textures);
}

void UAdvancedOptionsWidget::HandlePostProcessingQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetPostProcessingQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetPostProcessingQuality(SelectedQuality);
	}
	UpdateCurrentPostProcessing(GameUserSettings);

	ToggleSubOptions(PostProcessingSub, PostProcessing);
}

void UAdvancedOptionsWidget::HandleFoliageQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetFoliageQuality() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetFoliageQuality(SelectedQuality);
	}
	UpdateCurrentFoliage(GameUserSettings);

	ToggleSubOptions(FoliageSub, Foliage);
}

void UAdvancedOptionsWidget::HandleResolutionScaleSliderValueChanged(float NewValue)
{
}
