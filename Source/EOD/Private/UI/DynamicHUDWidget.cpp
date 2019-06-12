// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicHUDWidget.h"
#include "DynamicSkillBarWidget.h"
#include "InventoryWidget.h"
#include "PlayerStatsWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "DialogueWindowWidget.h"
#include "ContainerDragDropOperation.h"
#include "ContainerWidget.h"
#include "NotificationWidget.h"
#include "EODPlayerController.h"
#include "GameplayEffectBase.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

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
		SkillBarWidget &&
		PlayerLevel &&
		PlayerName)
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

void UDynamicHUDWidget::AddGameplayEffectUI(UGameplayEffectBase* GameplayEffect)
{
	UClass* GEWClass = GameplayEffectWidgetClass.Get();
	if (!GameplayEffect || !MainCanvas || !GEWClass)
	{
		return;
	}

	UContainerWidget* ContainerWidget = CreateWidget<UContainerWidget>(GetOwningPlayer(), GEWClass);
	if (ContainerWidget)
	{
		FContainerData ContainerData;
		ContainerData.Icon = GameplayEffect->Icon;
		ContainerWidget->SetContainerData(ContainerData);

		UPanelSlot* PSlot = MainCanvas->AddChild(ContainerWidget);
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PSlot);
		if (CanvasSlot)
		{
			CanvasSlot->SetSize(FVector2D(48.f, 48.f));
			CanvasSlot->SetPosition(FVector2D(1900.f, 20.f));
		}
	}

	GameplayEffectUIMap.Add(GameplayEffect, ContainerWidget);
}

void UDynamicHUDWidget::RemoveGameplayEffectUI(UGameplayEffectBase* GameplayEffect)
{
	UContainerWidget* ContWidget = GameplayEffectUIMap.Contains(GameplayEffect) ? GameplayEffectUIMap[GameplayEffect] : nullptr;
	if (ContWidget)
	{
		ContWidget->RemoveFromParent();
		// ContWidget->MarkPendingKill();
	}
	GameplayEffectUIMap.Remove(GameplayEffect);
}

void UDynamicHUDWidget::SetPlayerLevel(int32 Level)
{
	if (PlayerLevel)
	{
		PlayerLevel->SetText(FText::FromString(FString("Lv. ") + FString::FromInt(Level)));
	}
}

void UDynamicHUDWidget::SetPlayerName(FString Name)
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(Name));
	}
}

void UDynamicHUDWidget::DisplayNotification(UNotificationWidget* NotificationWidget)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (NotificationWidget)
	{
		for (TPair<UNotificationWidget*, UCanvasPanelSlot*> NotificationPair : Notifications)
		{
			if (NotificationPair.Value)
			{
				FVector2D OldPosition = NotificationPair.Value->GetPosition();
				FVector2D NewPosition = OldPosition + FVector2D(0.f, -84.f);
				NotificationPair.Value->SetPosition(NewPosition);
			}
		}

		if (MainCanvas)
		{
			UCanvasPanelSlot* NotificationSlot = MainCanvas->AddChildToCanvas(NotificationWidget);
			if (NotificationSlot)
			{
				NotificationSlot->SetSize(FVector2D(500.f, 64.f));
				NotificationSlot->SetPosition(FVector2D(-525.f, -100.f));
				NotificationSlot->SetAnchors(FAnchors(1.f, 1.f, 1.f, 1.f));
				UGameplayStatics::PlaySound2D(this, NotificationSound);

				Notifications.Add(NotificationWidget, NotificationSlot);

				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUObject(this, &UDynamicHUDWidget::RemoveNotification, NotificationWidget);

				FTimerHandle TimerHandle;
				World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 10.f, false);
			}
		}
	}
}

void UDynamicHUDWidget::RemoveNotification(UNotificationWidget* NotificationWidget)
{
	check(NotificationWidget);
	Notifications.Remove(NotificationWidget);
	NotificationWidget->DeleteNotification();
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
			return true;
		}
	}

	return false;
}
