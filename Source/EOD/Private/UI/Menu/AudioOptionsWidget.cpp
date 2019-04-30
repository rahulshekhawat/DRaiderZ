// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AudioOptionsWidget.h"
#include "EODLibrary.h"
#include "QualitySettingSubWidget.h"
#include "ScrollButtonWidget.h"

#include "GameFramework/GameUserSettings.h"

UAudioOptionsWidget::UAudioOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAudioOptionsWidget::Initialize()
{
	if (Super::Initialize() &&
		AudioQuality)
	{
		AudioQuality->OnClicked.AddDynamic(this, &UAudioOptionsWidget::HandleAudioQualityButtonClicked);
		AudioQualitySub->OnQualitySelected.AddDynamic(this, &UAudioOptionsWidget::HandleAudioQualitySelected);

		InitializeOptions(true);
		return true;
	}

	return false;
}

void UAudioOptionsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	check(AudioQualitySub);

	AudioQualitySub->SetCustomParentWidget(this);
}

void UAudioOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeOptions();
}

void UAudioOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UAudioOptionsWidget::InitializeOptions(bool bForceUserSettingReload)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->LoadSettings(bForceUserSettingReload);
		UpdateCurrentAudioQuality(GameUserSettings);
	}
}

void UAudioOptionsWidget::UpdateCurrentAudioQuality(UGameUserSettings* GameUserSettings)
{
	if (GameUserSettings == nullptr)
	{
		GameUserSettings = UEODLibrary::GetGameUserSettings();
	}

	check(GameUserSettings);
	check(AudioQuality);

	int32 Quality = GameUserSettings->GetAudioQualityLevel();
	FText Text = FText::FromString("N/A");
	GetQualityText(Quality, Text);
	AudioQuality->SetInfoText(Text);
}

void UAudioOptionsWidget::HandleAudioQualityButtonClicked()
{
	ToggleSubOptions(AudioQualitySub, AudioQuality);
}

void UAudioOptionsWidget::HandleAudioQualitySelected(int32 SelectedQuality)
{
	UGameUserSettings* GameUserSettings = UEODLibrary::GetGameUserSettings();
	if (GameUserSettings && GameUserSettings->GetAudioQualityLevel() != SelectedQuality)
	{
		bIsDirty = true;
		GameUserSettings->SetAudioQualityLevel(SelectedQuality);
	}
	UpdateCurrentAudioQuality(GameUserSettings);

	ToggleSubOptions(AudioQualitySub, AudioQuality);
}
