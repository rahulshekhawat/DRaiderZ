// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "TooltipWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"

UTooltipWidget::UTooltipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

bool UTooltipWidget::Initialize()
{
	if (Super::Initialize() &&
		Icon &&
		TitleText &&
		SubTitleText &&
		DescriptionText &&
		StatsVerticalBox)
	{
		return true;
	}

	return false;
}

void UTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTooltipWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTooltipWidget::SetIcon(UTexture* InTexture)
{
	check(Icon);
	FSlateBrush SlateBrush;
	SlateBrush.ImageSize = FVector2D(48.0, 48.0);
	SlateBrush.DrawAs = ESlateBrushDrawType::Image;
	SlateBrush.ImageType = ESlateBrushImageType::FullColor;
	SlateBrush.SetResourceObject(InTexture);
	Icon->SetBrush(SlateBrush);
}

void UTooltipWidget::SetTitle(const FString& InValue, ERarity Rarity)
{
	check(TitleText);
	FString Value = TEXT("<TITLE>") + InValue + TEXT("</>");
	TitleText->SetText(FText::FromString(Value));
}

void UTooltipWidget::SetSubTitle(const FString& InValue)
{
	check(SubTitleText);
	FString Value = TEXT("<SUBTEXT>") + InValue + TEXT("</>");
	SubTitleText->SetText(FText::FromString(Value));
}

void UTooltipWidget::SetDescription(const FString& InValue)
{
	check(DescriptionText);
	DescriptionText->SetText(FText::FromString(InValue));
}
