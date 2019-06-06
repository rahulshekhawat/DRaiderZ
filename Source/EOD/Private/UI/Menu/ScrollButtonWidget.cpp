// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ScrollButtonWidget.h"

#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/Slider.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UScrollButtonWidget::UScrollButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsChecked = false;
	DisplayText = FText::FromString(FString("BUTTON"));
	InfoText = FText::FromString(FString(""));
	bHasCheckBox = false;
	bHasSlider = false;
}

bool UScrollButtonWidget::Initialize()
{
	if (Super::Initialize() &&
		DisplayTextBlock &&
		SliderSizeBox &&
		InfoTextBlock &&
		CheckBoxImage &&
		SliderProgressBar &&
		OptionsSlider &&
		MainButton &&
		RootBorder &&
		SliderValueTextBlock)
	{
		MainButton->OnClicked.AddDynamic(this, &UScrollButtonWidget::OnMainButtonClicked);
		OptionsSlider->OnValueChanged.AddDynamic(this, &UScrollButtonWidget::OnOptionSliderValueChanged);

		return true;
	}

	return false;
}

void UScrollButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	DisplayTextBlock->SetText(DisplayText);
	if (bHasCheckBox)
	{
		InfoTextBlock->SetText(FText::FromString(FString("")));
		SliderSizeBox->SetVisibility(ESlateVisibility::Collapsed);
		CheckBoxImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		CheckBoxImage->SetVisibility(ESlateVisibility::Collapsed);

		if (bHasSlider)
		{
			SliderSizeBox->SetVisibility(ESlateVisibility::Visible);
			float SliderValue = OptionsSlider->GetValue();
			FText TempText = FText::FromString(FString::SanitizeFloat(SliderValue));
			InfoTextBlock->SetText(TempText);

			SliderProgressBar->SetPercent(SliderValue);
			InfoTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			SliderSizeBox->SetVisibility(ESlateVisibility::Collapsed);
			InfoTextBlock->SetText(InfoText);
			InfoTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UScrollButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScrollButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UScrollButtonWidget::SetDisplayText(FText NewText)
{
	if (DisplayTextBlock)
	{
		DisplayText = NewText;
		DisplayTextBlock->SetText(DisplayText);
	}
}

void UScrollButtonWidget::SetInfoText(FText NewText)
{
	if (InfoTextBlock)
	{
		InfoText = NewText;
		InfoTextBlock->SetText(NewText);
	}
}

void UScrollButtonWidget::SetChecked(bool bValue)
{
	if (bIsChecked != bValue && CheckBoxImage)
	{
		bIsChecked = bValue;
		if (bValue)
		{
			CheckBoxImage->SetBrushFromMaterial(CheckedMaterialInterface);
		}
		else
		{
			CheckBoxImage->SetBrushFromMaterial(UncheckedMaterialInterface);
		}
	}
}

void UScrollButtonWidget::SetSliderValue(float InValue)
{
	if (OptionsSlider)
	{
		OptionsSlider->SetValue(InValue);
	}

	if (SliderProgressBar)
	{
		SliderProgressBar->SetPercent(InValue);
	}

	if (SliderValueTextBlock)
	{
		SliderValueTextBlock->SetText(FText::FromString(FString::FromInt(InValue * 100)));
	}
}

FText UScrollButtonWidget::GetText() const
{
	return DisplayText;
}

bool UScrollButtonWidget::IsOptionChecked() const
{
	return bIsChecked;
}

void UScrollButtonWidget::OnMainButtonClicked()
{
	if (bHasCheckBox)
	{
		SetChecked(!bIsChecked);
	}

	OnClicked.Broadcast();
}

void UScrollButtonWidget::OnOptionSliderValueChanged(float NewValue)
{
	OnSliderValueChanged.Broadcast(NewValue);
	if (SliderProgressBar)
	{
		SliderProgressBar->SetPercent(NewValue);
	}
	if (SliderValueTextBlock)
	{
		FString SliderString = FString::FromInt(NewValue * 100) + FString("%");
		SliderValueTextBlock->SetText(FText::FromString(SliderString));
	}
}

void UScrollButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (RootBorder)
	{
		RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		RootBorder->SetPadding(FMargin(4.f, 4.f, 4.f, 4.f));
	}
}

void UScrollButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (RootBorder)
	{
		RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		RootBorder->SetPadding(FMargin(1.f, 1.f, 1.f, 0.f));
	}
}
