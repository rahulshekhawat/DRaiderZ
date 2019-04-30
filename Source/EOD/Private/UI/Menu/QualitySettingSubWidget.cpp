// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "QualitySettingSubWidget.h"
#include "ScrollButtonWidget.h"

UQualitySettingSubWidget::UQualitySettingSubWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UQualitySettingSubWidget::Initialize()
{
	if (Super::Initialize() &&
		Low &&
		Medium &&
		High &&
		Epic &&
		Cinematic)
	{
		Low->OnClicked.AddDynamic(this, &UQualitySettingSubWidget::HandleLowButtonClicked);
		Medium->OnClicked.AddDynamic(this, &UQualitySettingSubWidget::HandleMediumButtonClicked);
		High->OnClicked.AddDynamic(this, &UQualitySettingSubWidget::HandleHighButtonClicked);
		Epic->OnClicked.AddDynamic(this, &UQualitySettingSubWidget::HandleEpicButtonClicked);
		Cinematic->OnClicked.AddDynamic(this, &UQualitySettingSubWidget::HandleCinematicButtonClicked);

		return true;
	}

	return false;
}

void UQualitySettingSubWidget::NativeConstruct()
{
}

void UQualitySettingSubWidget::NativeDestruct()
{
}

void UQualitySettingSubWidget::SetCustomParentWidget(UUserWidget* NewParent)
{
	CustomParentWidget = NewParent;
}

void UQualitySettingSubWidget::HandleLowButtonClicked()
{
	OnQualitySelected.Broadcast(0);
}

void UQualitySettingSubWidget::HandleMediumButtonClicked()
{
	OnQualitySelected.Broadcast(1);
}

void UQualitySettingSubWidget::HandleHighButtonClicked()
{
	OnQualitySelected.Broadcast(2);
}

void UQualitySettingSubWidget::HandleEpicButtonClicked()
{
	OnQualitySelected.Broadcast(3);
}

void UQualitySettingSubWidget::HandleCinematicButtonClicked()
{
	OnQualitySelected.Broadcast(4);
}
