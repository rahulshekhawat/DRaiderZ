// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MenuButtonWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UMenuButtonWidget::UMenuButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsChecked = false;
	DisplayText = FText::FromString(FString("BUTTON"));
	InfoText = FText::FromString(FString(""));
	bHasCheckBox = false;
	bHasSlider = false;
}

bool UMenuButtonWidget::Initialize()
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

		MainButton->OnClicked.AddDynamic(this, &UMenuButtonWidget::OnMainButtonClicked);
		OptionsSlider->OnValueChanged.AddDynamic(this, &UMenuButtonWidget::OnOptionSliderValueChanged);

		return true;
	}

	return false;
}

void UMenuButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMenuButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMenuButtonWidget::OnMainButtonClicked()
{
	if (bHasCheckBox)
	{
		SetChecked(!bIsChecked);
	}

	OnClicked.Broadcast();
}

void UMenuButtonWidget::OnOptionSliderValueChanged(float NewValue)
{
	OnSliderValueChanged.Broadcast(NewValue);
	SliderProgressBar->SetPercent(NewValue);
	FString SliderString = FString::SanitizeFloat(NewValue * 100.f) + FString("%");
	SliderValueTextBlock->SetText(FText::FromString(SliderString));
}

void UMenuButtonWidget::SetInfoText(FText NewText)
{
	InfoText = NewText;
	InfoTextBlock->SetText(NewText);
}

void UMenuButtonWidget::SetChecked(bool bValue)
{
	if (bIsChecked != bValue)
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

void UMenuButtonWidget::SetSliderValue(float InValue)
{
	OptionsSlider->SetValue(InValue);
	SliderProgressBar->SetPercent(InValue);
	SliderValueTextBlock->SetText(FText::FromString(FString::SanitizeFloat(InValue * 100)));
}

void UMenuButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (RootBorder)
	{
		RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		RootBorder->SetPadding(FMargin(4.f, 4.f, 4.f, 4.f));
	}
}

void UMenuButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (RootBorder)
	{
		RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		RootBorder->SetPadding(FMargin(1.f, 1.f, 1.f, 0.f));
	}
}
