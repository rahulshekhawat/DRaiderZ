// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ResolutionSubWidget.h"
#include "EODLibrary.h"
#include "ResolutionButtonWidget.h"

#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "GameFramework/GameUserSettings.h"

UResolutionSubWidget::UResolutionSubWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UResolutionSubWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UResolutionSubWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	GenerateAvailableResolutionButtons();
}

void UResolutionSubWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UResolutionSubWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UResolutionSubWidget::SetParentOptionsWidget(UVideoOptionsWidget* NewParent)
{
	ParentOptionsWidget = NewParent;
}

void UResolutionSubWidget::GenerateAvailableResolutionButtons()
{
	FScreenResolutionArray ResolutionsArray;
	UClass* ButtonClass = ScrollButtonWidgetClass.Get();
	APlayerController* OwningPlayer = GetOwningPlayer();
	if (RHIGetAvailableResolutions(ResolutionsArray, true) && ButtonClass)  // needs the "RHI" dependency
	{
		for (const FScreenResolutionRHI& Resolution : ResolutionsArray)
		{
			if (Resolution.Width < MIN_SCREEN_WIDTH || Resolution.Height < MIN_SCREEN_HEIGHT)
			{
				continue;
			}

			FIntPoint IntResolution = FIntPoint(Resolution.Width, Resolution.Height);
			AvailableResolutions.Add(IntResolution);

			FString ResolutionStr = FString::FromInt(Resolution.Width) + "x" + FString::FromInt(Resolution.Height);
			UResolutionButtonWidget* ButtonWidget = CreateWidget<UResolutionButtonWidget>(OwningPlayer, ButtonClass);
			if (ButtonWidget)
			{
				ButtonWidget->ButtonResolution = IntResolution;
				ButtonWidget->SetDisplayText(FText::FromString(ResolutionStr));
				UScrollBoxSlot* ButtonSlot = Cast<UScrollBoxSlot>(ResolutionScroller->AddChild(ButtonWidget));
				if (ButtonSlot)
				{
					ButtonSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					ButtonSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}
				ButtonWidget->OnClicked_ResolutionPayload.AddDynamic(this, &UResolutionSubWidget::HandleResolutionButtonClicked);

				ResolutionWidgets.AddUnique(ButtonWidget);
			}
		}
	}
}

void UResolutionSubWidget::HandleResolutionButtonClicked(FIntPoint ButtonResolution)
{
	OnResolutionSelected.Broadcast(ButtonResolution);
}
