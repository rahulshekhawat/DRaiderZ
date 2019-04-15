// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicHUDWidget.h"
#include "DynamicSkillBarWidget.h"
#include "InventoryWidget.h"
#include "PlayerStatsWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "DialogueWindowWidget.h"
#include "ContainerDragDropOperation.h"
#include "ContainerWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UDynamicHUDWidget::UDynamicHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillBarWidgetSize = FVector2D(480.f, 96.f);
	SkillBarWidgetPosition = FVector2D(-240.f, -96.f);
	SkillBarWidgetAnchor = FAnchors(0.5f, 1.f, 0.5f, 1.f);
	DialogueWidgetSize = FVector2D(720.f, 360.f);
	DialogueWidgetPosition = FVector2D(600.f, 630.f);
}

bool UDynamicHUDWidget::Initialize()
{
	if (Super::Initialize() &&
		MainCanvas &&
		StatusIndicatorWidget &&
		InventoryWidget &&
		PlayerStatsWidget &&
		SkillTreeWidget &&
		SkillBarWidget)
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		PlayerStatsWidget->SetVisibility(ESlateVisibility::Hidden);

		return true;
	}

	return false;
}

void UDynamicHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDynamicHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDynamicHUDWidget::AddSkillBarWidget(UDynamicSkillBarWidget* NewWidget)
{
	if (SkillBarWidget == NewWidget || NewWidget == nullptr)
	{
		return;
	}

	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	if (CPSlot)
	{
		//~ @todo Set skill bar widget size to fit the number of available skills?
		CPSlot->SetSize(SkillBarWidgetSize);
		CPSlot->SetPosition(SkillBarWidgetPosition);
		CPSlot->SetAnchors(SkillBarWidgetAnchor);
	}
	SkillBarWidget = NewWidget;
}

void UDynamicHUDWidget::AddDialogueWidget(UDialogueWindowWidget* NewWidget)
{
	// @todo - improve?

	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	if (CPSlot)
	{
		CPSlot->SetSize(DialogueWidgetSize);
		CPSlot->SetPosition(DialogueWidgetPosition);
	}
}

bool UDynamicHUDWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UContainerDragDropOperation* Operation = Cast<UContainerDragDropOperation>(InOperation);
	if (!Operation || !Operation->DraggedContainerWidget)
	{
		return false;
	}

	if (Operation->DraggedContainerWidget->GetContainerType() == EContainerType::SkillBar)
	{
		UDynamicSkillBarWidget* ParentSkillBarWidget = Cast<UDynamicSkillBarWidget>(Operation->DraggedContainerWidget->GetContainerParent());
		if (ParentSkillBarWidget)
		{
			ParentSkillBarWidget->OnContainerRemoved(Operation->DraggedContainerWidget);
		}
	}

	return true;
}
