// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidgetBase.h"
#include "TooltipWidget.h"

UContainerWidgetBase::UContainerWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UContainerWidgetBase::Initialize()
{
	if (Super::Initialize() &&
		RootBorder &&
		MainButton &&
		ItemImage &&
		SubText &&
		CooldownText)
	{
		InitializeTooltipWidget();

		SubText->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);

		return true;
	}

	return false;
}

void UContainerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UContainerWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContainerWidgetBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UContainerWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UContainerWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}

void UContainerWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

FReply UContainerWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UContainerWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UContainerWidgetBase::PostManualConstruction(UUserWidget* InParentWidget, UObject* InDataObj)
{

	// check(ParentWidget); // ParentWidget shouldn't be NULL

	/*
	ContainerParentWidget = ParentWidget;
	if (ParentWidget->IsA(UDynamicSkillTreeWidget::StaticClass()))
	{
		ContainerType = EContainerType::SkillTree;
		bDisplaySubTextAsRatio = true;
	}
	else if (ParentWidget->IsA(UDynamicSkillBarWidget::StaticClass()))
	{
		ContainerType = EContainerType::SkillBar;
	}
	*/
}

void UContainerWidgetBase::ResetContainer()
{
}

void UContainerWidgetBase::SetDataObj(UObject* InDataObj)
{
}

bool UContainerWidgetBase::IsContainerEmpty() const
{
	return (DataObj != nullptr);
}

void UContainerWidgetBase::SetIcon(UTexture* NewIcon)
{
	// If the NewIcon is a valid texture
	if (NewIcon)
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.ImageType = ESlateBrushImageType::FullColor;
		SlateBrush.SetResourceObject(NewIcon);
		ItemImage->SetBrush(SlateBrush);
	}
	// If the NewIcon is NULL, we simply remove the previously set image inside ItemImage
	else
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		SlateBrush.ImageType = ESlateBrushImageType::NoImage;
		ItemImage->SetBrush(SlateBrush);
	}
}

void UContainerWidgetBase::SetSubText(int32 InCurrentValue, int32 InMaxValue)
{
}

void UContainerWidgetBase::SetCooldown(float InCooldown)
{
}

UWidget* UContainerWidgetBase::GetTooltipWidget()
{
	if (DataObj.Get())
	{
		return ToolTipWidget;
	}

	return nullptr;
}

void UContainerWidgetBase::InitializeTooltipWidget()
{
	if (TooltipWidgetClass.Get())
	{
		ToolTipWidget = CreateWidget<UUserWidget>(this, TooltipWidgetClass.Get());
	}

	if (!RootBorder->ToolTipWidgetDelegate.IsBoundToObject(this))
	{
		RootBorder->ToolTipWidgetDelegate.BindUFunction(this, TEXT("GetTooltipWidget"));
	}
}

void UContainerWidgetBase::UpdateTooltipWidget()
{
}
