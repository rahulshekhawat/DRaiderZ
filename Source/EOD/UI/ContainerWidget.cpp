// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "EODGlobalNames.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

UContainerWidget::UContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EContainerType::None;
	bDisplaySubTextAsRatio = false;
}

bool UContainerWidget::Initialize()
{
	if (Super::Initialize() && SubText && CooldownText && ItemImage)
	{
		SubText->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);

		Internal_InitializeContainer();
		return true;
	}
	return false;
}

void UContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Internal_InitializeContainer();
}

void UContainerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContainerWidget::InitializeWithParent(UUserWidget* ParentWidget)
{
	check(ParentWidget); // ParentWidget shouldn't be NULL

	if (ParentWidget->StaticClass() == UDynamicSkillTreeWidget::StaticClass())
	{
		ContainerType = EContainerType::SkillTree;
		bDisplaySubTextAsRatio = true;
	}
}

void UContainerWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UContainerWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}

void UContainerWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

FReply UContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();
	return Reply;
}

FReply UContainerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();
	return Reply;
}

void UContainerWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
}

void UContainerWidget::SetCanBeClicked(bool bValue)
{
	bCanBeClicked = bValue;
	//~ @todo Update container's border material
}

void UContainerWidget::SetCanBeDragged(bool bValue)
{
	bCanBeDragged = bValue;
	//~ @todo Update container's border material
}

void UContainerWidget::SetDescription(const FString& NewDescription)
{
}

void UContainerWidget::SetIcon(UTexture* NewIcon)
{
	if (ItemImage)
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

		ContainerData.Icon = NewIcon;
	}
}

void UContainerWidget::SetInGameName(const FString& NewInGameName)
{
}

void UContainerWidget::SetItemType(EEODItemType EODItemType)
{
}

void UContainerWidget::SetCurrentValue(int32 NewValue)
{
}

void UContainerWidget::SetMaxValue(int32 NewValue)
{
}

void UContainerWidget::SetItemID(FName NewID)
{
}

void UContainerWidget::SetItemGroup(FName NewItemGroup)
{
}

void UContainerWidget::Internal_InitializeContainer()
{
	if (BorderImage)
	{
		if (!EmptyBorderMID)
		{
			EmptyBorderMID = BorderImage->GetDynamicMaterial();
		}

		if (EmptyBorderMID)
		{
			EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
		}
	}
}
