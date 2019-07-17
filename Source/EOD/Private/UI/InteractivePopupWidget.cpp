// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "InteractivePopupWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"

UInteractivePopupWidget::UInteractivePopupWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UInteractivePopupWidget::Initialize()
{
	if (Super::Initialize() &&
		IconImage &&
		KeyText &&
		DetailText)
	{
		return true;
	}

	return false;
}

void UInteractivePopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInteractivePopupWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UInteractivePopupWidget::SetDetailText(FText Text)
{
	check(DetailText);
	DetailText->SetText(Text);
}

void UInteractivePopupWidget::SetKeyText(FText Text)
{
	check(KeyText);
	KeyText->SetText(Text);
}

void UInteractivePopupWidget::SetIcon(UTexture* InTexture)
{
	if (InTexture)
	{
		check(IconImage);
		FSlateBrush SlateBrush;
		SlateBrush.SetResourceObject(InTexture);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.Tiling = ESlateBrushTileType::NoTile;
		IconImage->SetBrush(SlateBrush);
	}
}
