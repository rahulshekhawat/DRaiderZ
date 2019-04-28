// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ResolutionWidget.h"
#include "EODLibrary.h"
#include "ScrollButtonWidget.h"

#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "GameFramework/GameUserSettings.h"

UResolutionWidget::UResolutionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UResolutionWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UResolutionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	GenerateAvailableResolutionButtons();
}

void UResolutionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UResolutionWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UResolutionWidget::GenerateAvailableResolutionButtons()
{
	FScreenResolutionArray ResolutionsArray;
	if (RHIGetAvailableResolutions(ResolutionsArray, true))  // needs the "RHI" dependency
	{
		for (const FScreenResolutionRHI& Resolution : ResolutionsArray)
		{
			if (Resolution.Width < MIN_SCREEN_WIDTH || Resolution.Height < MIN_SCREEN_HEIGHT)
			{
				continue;
			}

			FString Str = FString::FromInt(Resolution.Width) + "x" + FString::FromInt(Resolution.Height);
			AvailableResolutions.AddUnique(Str);
		}
	}

	UClass* ButtonClass = ScrollButtonWidgetClass.Get();
	APlayerController* OwningPlayer = GetOwningPlayer();
	if (ButtonClass && AvailableResolutions.Num() > 0)
	{
		for (FString Resolution : AvailableResolutions)
		{
			UScrollButtonWidget* ButtonWidget = CreateWidget<UScrollButtonWidget>(OwningPlayer, ButtonClass);
			if (ButtonWidget)
			{
				ResolutionWidgets.AddUnique(ButtonWidget);
				ButtonWidget->SetDisplayText(FText::FromString(Resolution));
				UScrollBoxSlot* ButtonSlot = Cast<UScrollBoxSlot>(ResolutionScroller->AddChild(ButtonWidget));
				if (ButtonSlot)
				{
					ButtonSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					ButtonSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}
			}
		}
	}
}
