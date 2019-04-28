// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ScrollButtonWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UScrollButtonWidget::UScrollButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TextToDisplay = FText::FromString(FString("Regular Buttom"));
}

bool UScrollButtonWidget::Initialize()
{
	if (Super::Initialize() &&
		DisplayText &&
		InfoText &&
		SliderSizeBox &&
		CheckBoxImage)
	{
		DisplayText->SetText(TextToDisplay);

		if (bHasCheckBox)
		{
			InfoText->SetText(FText());
			SliderSizeBox->SetVisibility(ESlateVisibility::Collapsed);
			CheckBoxImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CheckBoxImage->SetVisibility(ESlateVisibility::Collapsed);

		}

		return true;
	}
	return false;
}

void UScrollButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UScrollButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScrollButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
