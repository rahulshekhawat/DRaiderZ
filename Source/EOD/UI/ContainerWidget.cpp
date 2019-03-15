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
}

bool UContainerWidget::Initialize()
{
	if (Super::Initialize() && SubText && CooldownText && ItemImage)
	{
		SubText->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);

		InitializeContainer();
		return true;
	}
	return false;
}

void UContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeContainer();
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

void UContainerWidget::UpdateDescription(const FString& NewDescription)
{
}

void UContainerWidget::InitializeContainer()
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
