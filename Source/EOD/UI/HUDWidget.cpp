// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HUDWidget.h"
#include "SkillBarWidget.h"
#include "SkillTreeWidget.h"
#include "InventoryWidget.h"
#include "StatusIndicatorWidget.h"
#include "EODItemDragDropOperation.h"
#include "EODItemContainer.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
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

FORCEINLINE void UHUDWidget::UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth)
{
	StatusIndicatorWidget->UpdateHealthBar(CurrentHealth, MaxHealth, BaseHealth);
}

FORCEINLINE void UHUDWidget::UpdateManaBar(int32 CurrentMana, int32 MaxMana, int32 BaseMana)
{
	StatusIndicatorWidget->UpdateManaBar(CurrentMana, MaxMana, BaseMana);
}

FORCEINLINE void UHUDWidget::UpdateStaminaBar(int32 CurrentStamina, int32 MaxStamina, int32 BaseStamina)
{
	StatusIndicatorWidget->UpdateStaminaBar(CurrentStamina, MaxStamina, BaseStamina);
}

FORCEINLINE FName UHUDWidget::GetSkillAtIndex(int32 SkillIndex) const
{
	return SkillBarWidget->GetSkillAtIndex(SkillIndex);
}

bool UHUDWidget::IsSkillInCooldown(int32 SkillIndex) const
{
	return SkillBarWidget->IsSkillInCooldown(SkillIndex);
}

FORCEINLINE void UHUDWidget::PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval)
{
	SkillBarWidget->PutSkillOnCooldownTimer(SkillIndex, Duration, Interval);
}

FORCEINLINE void UHUDWidget::SaveHUDLayout()
{
	SkillBarWidget->SaveSkillBarLayout();
}

FORCEINLINE void UHUDWidget::AddSkillBarWidget(USkillBarWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(SkillBarWidgetSize);
	CPSlot->SetPosition(SkillBarWidgetPosition);
	CPSlot->SetAnchors(SkillBarWidgetAnchor);

	SkillBarWidget = NewWidget;
}

FORCEINLINE void UHUDWidget::AddSkillTreeWidget(USkillTreeWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(SkillTreeWidgetSize);
	CPSlot->SetPosition(SkillTreeWidgetPosition);

	SkillTreeWidget = NewWidget;
}

FORCEINLINE void UHUDWidget::AddInventoryWidget(UInventoryWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(InventoryWidgetSize);
	CPSlot->SetPosition(InventoryWidgetPosition);

	InventoryWidget = NewWidget;
}

FORCEINLINE void UHUDWidget::AddStatusIndicatorWidget(UStatusIndicatorWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(StatusIndicatorWidgetSize);
	CPSlot->SetPosition(StatusIndicatorWidgetPosition);

	StatusIndicatorWidget = NewWidget;
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
