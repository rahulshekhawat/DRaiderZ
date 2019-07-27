// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ItemInfoWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"

UItemInfoWidget::UItemInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultItemName = FText::FromString(TEXT("<TITLE>Cyndea's Greatsword</>"));
	DefaultItemSubText = FText::FromString(TEXT("x1"));
	bSelected = false;
	DefaultIcon = nullptr;
}

bool UItemInfoWidget::Initialize()
{
	if (Super::Initialize() &&
		BackgroundButton &&
		ItemIcon &&
		TitleText &&
		SubText)
	{
		BackgroundButton->OnPressed.AddUniqueDynamic(this, &UItemInfoWidget::OnBackgroundButtonPressed);

		SetIcon(DefaultIcon);
		TitleText->SetText(DefaultItemName);
		SubText->SetText(DefaultItemSubText);
		return true;
	}

	return false;
}

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UItemInfoWidget::OnBackgroundButtonPressed()
{
	OnPressed.Broadcast(this);
}

void UItemInfoWidget::SetItemName(FString InValue)
{
	check(TitleText);
	FString Str = TEXT("<TITLE>") + InValue + TEXT("</>");
	TitleText->SetText(FText::FromString(Str));
}

void UItemInfoWidget::SetItemSubText(FString InValue)
{
	check(SubText);
	SubText->SetText(FText::FromString(InValue));
}

void UItemInfoWidget::SetIcon(UTexture* InIcon)
{
	check(ItemIcon);
	// If the NewIcon is a valid texture
	if (InIcon)
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(48.0, 48.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.ImageType = ESlateBrushImageType::FullColor;
		SlateBrush.SetResourceObject(InIcon);
		ItemIcon->SetBrush(SlateBrush);
	}
	// If the NewIcon is NULL, we simply remove the previously set image inside ItemImage
	else
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(48.0, 48.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		SlateBrush.ImageType = ESlateBrushImageType::NoImage;
		ItemIcon->SetBrush(SlateBrush);
	}
}

void UItemInfoWidget::SetSelected(bool bValue)
{
	check(BackgroundButton);
	if (bValue)
	{
		BackgroundButton->SetBackgroundColor(FLinearColor(0.1f, 0.15f, 0.16f, 1.f));
	}
	else
	{
		BackgroundButton->SetBackgroundColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.f));
	}
	bSelected = bValue;
}

bool UItemInfoWidget::IsSelected() const
{
	return bSelected;
}
