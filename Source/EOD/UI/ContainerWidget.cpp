// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "EODGlobalNames.h"
#include "EODItemDragDropOperation.h"
#include "EODPreprocessors.h"

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
}

void UContainerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContainerWidget::InitializeWithParent(UUserWidget* ParentWidget)
{
	check(ParentWidget); // ParentWidget shouldn't be NULL
	
	ContainerParentWidget = ParentWidget;
	if (ParentWidget->IsA(UDynamicSkillTreeWidget::StaticClass()))
	{
		ContainerType = EContainerType::SkillTree;
		bDisplaySubTextAsRatio = true;
	}
}

void UContainerWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Cannot drop anything on skill tree
	if (ContainerType == EContainerType::SkillTree)
	{
		return false;
	}

	bool bResult = false;

	/*
	UEODItemDragDropOperation* Operation = Cast<UEODItemDragDropOperation>(InOperation);
	if (!IsValid(Operation) || !IsValid(Operation->DraggedEODItemWidget))
	{
		return false;
	}


	bool bResult = false;
	// Cannot drop anything from skill tree to inventory
	// Cannot drop anything from inventory to skill bar
	// Cannot drop anything from skill bar to inventory
	if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillTree &&
		ContainerType == EEODContainerType::SkillBar)
	{
		USkillBarWidget* SkillBarWidget = Cast<USkillBarWidget>(ParentWidget);
		if (IsValid(SkillBarWidget))
		{
			bResult = SkillBarWidget->OnNewSkillDropped(Operation->DraggedEODItemWidget, this);
		}
	}
	else if ((Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::Inventory &&
		ContainerType == EEODContainerType::Inventory) ||
		(Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillBar &&
			ContainerType == EEODContainerType::SkillBar))
	{
		USkillBarWidget* SkillBarWidget = Cast<USkillBarWidget>(ParentWidget);
		if (IsValid(SkillBarWidget))
		{
			bResult = SkillBarWidget->OnSkillsSwapped(Operation->DraggedEODItemWidget, this);
		}
	}
	*/
	return bResult;
}

void UContainerWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, HoveredBorderColor);
	}
}

void UContainerWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
	}
}

FReply UContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bLMBDown = InMouseEvent.IsMouseButtonDown(FKey(KeyboardKeysNames::LeftMouseButton));
	bool bRMBDown = InMouseEvent.IsMouseButtonDown(FKey(KeyboardKeysNames::RightMouseButton));

	FReply Reply = FReply::Handled();
	if (bLMBDown && bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, PressedBorderColor);
		OnClicked.Broadcast(this, ContainerParentWidget);
	}
	else if (bRMBDown && bCanBeDragged && ContainerData.Icon != nullptr)
	{
		TSharedPtr<SWidget> SlateWidgetDetectingDrag = this->GetCachedWidget();
		if (SlateWidgetDetectingDrag.IsValid())
		{
			Reply = Reply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), InMouseEvent.GetEffectingButton());
		}
	}
	
	return Reply;
}

FReply UContainerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if ((bCanBeClicked || bCanBeDragged) && EmptyBorderMID)
	if (bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, HoveredBorderColor);
	}
	return FReply::Handled();
}

void UContainerWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);

	if (ItemImage)
	{
		ItemImage->SetIsEnabled(bInIsEnabled);
	}
}

void UContainerWidget::SetCanBeClicked(bool bValue)
{
	bCanBeClicked = bValue;

	if (!bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
	}
	//~ @todo Update container's border material
}

void UContainerWidget::SetCanBeDragged(bool bValue)
{
	bCanBeDragged = bValue;
	//~ @todo Update container's border material
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
	ToolTipText = FText::FromString(NewInGameName);
}

void UContainerWidget::SetDescription(const FString& NewDescription)
{
}

void UContainerWidget::SetItemType(EEODItemType EODItemType)
{
	ContainerData.EODItemType = EODItemType;
}

void UContainerWidget::SetCurrentValue(int32 NewValue)
{
	if (bDisplaySubTextAsRatio && SubText)
	{
		ContainerData.CurrentValue = NewValue;
		check(ContainerData.CurrentValue <= ContainerData.MaxValue);
		SubText->SetVisibility(ESlateVisibility::Visible);
		FString NewSubTextString = FString::FromInt(NewValue) + FString("/") + FString::FromInt(ContainerData.MaxValue);
		FText NewText = FText::FromString(NewSubTextString);
		SubText->SetText(NewText);
	}
	else if (!bDisplaySubTextAsRatio && SubText)
	{
		ContainerData.CurrentValue = NewValue;
		check(ContainerData.CurrentValue <= ContainerData.MaxValue);
		SubText->SetVisibility(ESlateVisibility::Visible);
		FString NewSubTextString = FString::FromInt(NewValue);
		FText NewText = FText::FromString(NewSubTextString);
		SubText->SetText(NewText);
	}
}

void UContainerWidget::SetMaxValue(int32 NewValue)
{
	if (bDisplaySubTextAsRatio && SubText)
	{
		ContainerData.MaxValue = NewValue;
		SubText->SetVisibility(ESlateVisibility::Visible);
		FString NewSubTextString = FString::FromInt(ContainerData.CurrentValue) + FString("/") + FString::FromInt(NewValue);
		FText NewText = FText::FromString(NewSubTextString);
		SubText->SetText(NewText);
	}
}

void UContainerWidget::SetItemID(FName NewID)
{
	ContainerData.ItemID = NewID;
}

void UContainerWidget::SetItemGroup(FName NewItemGroup)
{
	ContainerData.ItemGroup = NewItemGroup;
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

	SetItemID(ContainerData.ItemID);
	SetItemGroup(ContainerData.ItemGroup);
	SetIcon(ContainerData.Icon);
	SetInGameName(ContainerData.InGameName);
	SetDescription(ContainerData.Description);
	SetCurrentValue(ContainerData.CurrentValue);
	SetMaxValue(ContainerData.MaxValue);

	SetCanBeClicked(true);
	SetCanBeDragged(true);
	SetIsEnabled(true);
}
