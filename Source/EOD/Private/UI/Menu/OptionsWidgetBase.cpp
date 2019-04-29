// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "OptionsWidgetBase.h"
#include "ScrollButtonWidget.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

UOptionsWidgetBase::UOptionsWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UOptionsWidgetBase::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UOptionsWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOptionsWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UOptionsWidgetBase::InitializeOptions()
{
}

void UOptionsWidgetBase::CloseDownOptions()
{
}

void UOptionsWidgetBase::ResetOptions()
{
}

void UOptionsWidgetBase::EnableScrollBoxItems()
{
	if (OptionsScrollBox)
	{
		int32 ChildCount = OptionsScrollBox->GetChildrenCount();
		for (int i = 0; i < ChildCount; i++)
		{
			UWidget* Widget = OptionsScrollBox->GetChildAt(i);
			if (Widget)
			{
				Widget->SetIsEnabled(true);
			}
		}
	}
}

void UOptionsWidgetBase::DisableScrollBoxItems(UWidget* ExcludedItem)
{
	if (OptionsScrollBox)
	{
		int32 ChildCount = OptionsScrollBox->GetChildrenCount();
		for (int i = 0; i < ChildCount; i++)
		{
			UWidget* Widget = OptionsScrollBox->GetChildAt(i);
			if (Widget && Widget != ExcludedItem)
			{
				Widget->SetIsEnabled(false);
			}
		}
	}
}

void UOptionsWidgetBase::ToggleSubOptions(UWidget* SubWidget, UScrollButtonWidget* CallingScrollButton)
{
	check(SubWidgetSwitcher);
	check(SubWidget);
	check(CallingScrollButton);

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
		// MoveSubMenu(CallingScrollButton);
	}
}

void UOptionsWidgetBase::GetQualityText(int32 InQualityLevel, FText& OutText)
{
	switch (InQualityLevel)
	{
	case 0:
		OutText = FText::FromString("LOW");
		break;
	case 1:
		OutText = FText::FromString("MEDIUM");
		break;
	case 2:
		OutText = FText::FromString("HIGH");
		break;
	case 3:
		OutText = FText::FromString("EPIC");
		break;
	case 4:
		OutText = FText::FromString("CINEMATIC");
		break;
	default:
		break;
	}
}

void UOptionsWidgetBase::SetSettingsParentWidget(USettingsWidget* NewParent)
{
}
