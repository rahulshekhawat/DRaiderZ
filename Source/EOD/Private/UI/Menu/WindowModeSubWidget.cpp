// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "WindowModeSubWidget.h"
#include "ScrollButtonWidget.h"

UWindowModeSubWidget::UWindowModeSubWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UWindowModeSubWidget::Initialize()
{
	if (Super::Initialize() &&
		Window &&
		Fullscreen &&
		Borderless)
	{
		Window->OnClicked.AddDynamic(this, &UWindowModeSubWidget::HandleWindowButtonClicked);
		Fullscreen->OnClicked.AddDynamic(this, &UWindowModeSubWidget::HandleFullscreenButtonClicked);
		Borderless->OnClicked.AddDynamic(this, &UWindowModeSubWidget::HandleBorderlessButtonClicked);

		return true;
	}

	return false;
}

void UWindowModeSubWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWindowModeSubWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UWindowModeSubWidget::SetParentOptionsWidget(UVideoOptionsWidget* NewParent)
{
	ParentOptionsWidget = NewParent;
}

void UWindowModeSubWidget::HandleWindowButtonClicked()
{
	OnWindowModeSelected.Broadcast(EWindowMode::Windowed);
}

void UWindowModeSubWidget::HandleFullscreenButtonClicked()
{
	OnWindowModeSelected.Broadcast(EWindowMode::Fullscreen);
}

void UWindowModeSubWidget::HandleBorderlessButtonClicked()
{
	OnWindowModeSelected.Broadcast(EWindowMode::WindowedFullscreen);
}
