// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HUDWidget.h"
#include "EODItemDragDropOperation.h"

#include "Kismet/KismetSystemLibrary.h"

UHUDWidget::UHUDWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	SkillBarWidgetSize = FVector2D(480.f, 96.f);
	SkillBarWidgetPosition = FVector2D(-240.f, -96.f);
	SkillBarWidgetAnchor = FAnchors(0.5f, 1.f, 0.5f, 1.f);
	InventoryWidgetSize = FVector2D(384.f, 480.f);
	InventoryWidgetPosition = FVector2D(1468.f, 224.f);
	SkillTreeWidgetSize = FVector2D(900.f, 600.f);
	SkillTreeWidgetPosition = FVector2D(464.f, 220.f);
	StatusIndicatorWidgetSize = FVector2D(375.f, 50.f);
	StatusIndicatorWidgetPosition = FVector2D(74.f, 20.f);

}

bool UHUDWidget::Initialize()
{
	if (!(Super::Initialize()))
	{
		return false;
	}


	return true;
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	// InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHUDWidget::SaveHUDLayout()
{
	SkillBarWidget->SaveSkillBarLayout();
}

bool UHUDWidget::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	UEODItemDragDropOperation* Operation = Cast<UEODItemDragDropOperation>(InOperation);
	if (!Operation || !Operation->DraggedEODItemWidget)
	{
		return false;
	}

	if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillBar)
	{
		Operation->DraggedEODItemWidget->ResetContainer();
		Operation->DraggedEODItemWidget->RefreshContainerVisuals();
	}

	return true;
}
