// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ResolutionWidget.h"
#include "EODLibrary.h"

#include "Components/ScrollBox.h"
#include "GameFramework/GameUserSettings.h"

UResolutionWidget::UResolutionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

bool UResolutionWidget::Initialize()
{
	if (Super::Initialize())
	{
		GenerateAvailableResolutionButtons();
		return true;
	}

	return false;
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
	TArray<FString> Resolutions;
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

	for (FString Resolution : Resolutions)
	{
		AddResolutionButtonToScrollBox(Resolution);
	}

}
