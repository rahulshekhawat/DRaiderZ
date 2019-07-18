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

void UInteractivePopupWidget::ResetDetailText()
{
	check(DetailText);
	DetailText->SetText(FText::FromString(TEXT("Interact")));
}

void UInteractivePopupWidget::ResetKeyText()
{
	check(KeyText);
	FString SubTextString = TEXT("<SubText>E</>");
	KeyText->SetText(FText::FromString(SubTextString));
}

void UInteractivePopupWidget::ResetIcon()
{
	check(IconImage);
	FSlateBrush SlateBrush;
	SlateBrush.SetResourceObject(DefaultIcon);
	SlateBrush.DrawAs = ESlateBrushDrawType::Image;
	SlateBrush.Tiling = ESlateBrushTileType::NoTile;
	IconImage->SetBrush(SlateBrush);
}

UObject* UInteractivePopupWidget::GetRegisteringObject() const
{
	return RegisteringObj.Get();
}

bool UInteractivePopupWidget::IsRegistered() const
{
	if (this->IsVisible() && RegisteringObj.IsValid())
	{
		return true;
	}
	return false;
}

bool UInteractivePopupWidget::IsRegisteredWithObject(UObject* InObj) const
{
	if (InObj && this->IsVisible() && RegisteringObj.Get() == InObj)
	{
		return true;
	}
	return false;
}

void UInteractivePopupWidget::RegisterWithObject(UObject* Obj)
{
	if (Obj)
	{
		RegisteringObj = Obj;
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInteractivePopupWidget::Unregister()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	RegisteringObj = nullptr;
}
