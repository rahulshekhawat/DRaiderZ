// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidget.h"
#include "DynamicSkillTreeWidget.h"


UContainerWidget::UContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EContainerType::None;
}

bool UContainerWidget::Initialize()
{
	return false;
}

void UContainerWidget::NativeConstruct()
{
}

void UContainerWidget::NativeDestruct()
{
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
