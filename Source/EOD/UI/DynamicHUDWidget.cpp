// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicHUDWidget.h"
#include "SkillBarWidget.h"
#include "InventoryWidget.h"
#include "PlayerStatsWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "DialogueWindowWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UDynamicHUDWidget::UDynamicHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDynamicHUDWidget::Initialize()
{
	if (Super::Initialize() &&
		MainCanvas &&
		StatusIndicatorWidget &&
		InventoryWidget &&
		PlayerStatsWidget &&
		SkillTreeWidget)
	{
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

void UDynamicHUDWidget::AddSkillBarWidget(USkillBarWidget* NewWidget)
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

void UDynamicHUDWidget::AddSkillTreeWidget(UDynamicSkillTreeWidget* NewWidget)
{
	if (SkillTreeWidget == NewWidget || NewWidget == nullptr)
	{
		return;
	}

	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	if (CPSlot)
	{
		CPSlot->SetSize(SkillTreeWidgetSize);
		CPSlot->SetPosition(SkillTreeWidgetPosition);
	}
	SkillTreeWidget = NewWidget;
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
