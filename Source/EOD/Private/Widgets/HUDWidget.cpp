// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "HUDWidget.h"
#include "SkillBarWidget.h"
#include "InventoryWidget.h"
#include "PlayerStatsWidget.h"
#include "SkillTreeWidget.h"
#include "DialogueWindowWidget.h"
#include "ContainerDragDropOperation.h"
#include "ContainerWidget.h"
#include "NotificationWidget.h"
#include "EODPlayerController.h"
#include "GameplayEffectBase.h"
#include "InteractivePopupWidget.h"
#include "EODDragDropOperation.h"
#include "SkillBarContainerWidget.h"
#include "PlayerSkillBase.h"
#include "PlayerSkillsComponent.h"
#include "StatusEffectWidget.h"
#include "LootWidget.h"
#include "CraftWidget.h"
#include "StatusIndicatorWidget.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UHUDWidget::Initialize()
{
	if (Super::Initialize() &&
		MainCanvas &&
		StatusIndicatorWidget &&
		InventoryWidget &&
		PlayerStatsWidget &&
		SkillTreeWidget &&
		SkillBarWidget &&
		PlayerLevel &&
		PlayerName &&
		InteractivePopup &&
		StatusEffectsHBox)
	{
		InteractivePopup->SetVisibility(ESlateVisibility::Hidden);
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		PlayerStatsWidget->SetVisibility(ESlateVisibility::Hidden);

		return true;
	}

	return false;
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UHUDWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UEODDragDropOperation* DragOp = Cast<UEODDragDropOperation>(InOperation);
	if (DragOp == nullptr)
	{
		return false;
	}

	USkillBarContainerWidget* SourceCont = Cast<USkillBarContainerWidget>(DragOp->DragOpSourceContainer);
	if (SourceCont)
	{
		UPlayerSkillBase* SourceSkill = Cast<UPlayerSkillBase>(DragOp->Payload);
		check(SourceSkill);

		UPlayerSkillsComponent* SkillsComp = Cast<UPlayerSkillsComponent>(SourceSkill->InstigatorSkillComponent.Get());
		check(SkillsComp);

		SkillsComp->RemoveSkillFromSkillBar(SourceCont->SkillBarIndex, NAME_None);
		SourceCont->SetDataObj(nullptr);
	}
	return true;
}

void UHUDWidget::AddLootWidget(ULootWidget* InWidget)
{
	if (InWidget)
	{
		RemoveActiveLootWidget();
		ActiveLootWidget = InWidget;
		check(MainCanvas);
		UCanvasPanelSlot* LootSlot = MainCanvas->AddChildToCanvas(ActiveLootWidget);
		check(LootSlot);
		LootSlot->SetSize(FVector2D(400.f, 500.f));
		LootSlot->SetPosition(FVector2D(200.f, 300.f));
	}
}

void UHUDWidget::RemoveActiveLootWidget()
{
	if (ActiveLootWidget)
	{
		ActiveLootWidget->RemoveFromParent();
		ActiveLootWidget = nullptr;
	}
}

bool UHUDWidget::RemoveLootWidget(UObject* InLootSource)
{
	if (ActiveLootWidget && ActiveLootWidget->GetLootSource() == InLootSource)
	{
		ActiveLootWidget->RemoveFromParent();
		ActiveLootWidget = nullptr;
		return true;
	}

	return false;
}

/*
void UHUDWidget::SetPlayerLevel(int32 Level)
{
	check(PlayerLevel);
	PlayerLevel->SetText(FText::FromString(FString("Lv. ") + FString::FromInt(Level)));
}

void UHUDWidget::SetPlayerName(FString Name)
{
	check(PlayerName);
	PlayerName->SetText(FText::FromString(Name));
}
*/

void UHUDWidget::AddGameplayEffectUI(UGameplayEffectBase* GameplayEffect)
{
	if (GameplayEffect == nullptr)
	{
		return;
	}

	check(StatusEffectWidgetClass.Get());
	check(StatusEffectsHBox);
	UStatusEffectWidget* StatusEffectWidget = CreateWidget<UStatusEffectWidget>(GetOwningPlayer(), StatusEffectWidgetClass.Get());
	if (StatusEffectWidget)
	{
		StatusEffectWidget->SetDuration(GameplayEffect->GetDuration());
		StatusEffectWidget->SetIcon(GameplayEffect->Icon);

		UHorizontalBoxSlot* HBoxSlot = StatusEffectsHBox->AddChildToHorizontalBox(StatusEffectWidget);
		check(HBoxSlot);
		HBoxSlot->SetPadding(FMargin(-5.f, 0.f, -5.f, 0.f));		

		GameplayEffectWidgetsMap.Add(GameplayEffect, StatusEffectWidget);
	}
}

void UHUDWidget::RemoveGameplayEffectUI(UGameplayEffectBase* GameplayEffect)
{
	if (GameplayEffectWidgetsMap.Contains(GameplayEffect))
	{
		UStatusEffectWidget* Widget = GameplayEffectWidgetsMap[GameplayEffect];
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
		GameplayEffectWidgetsMap.Remove(GameplayEffect);
	}
}

void UHUDWidget::AddCraftWidget_Implementation(UCraftWidget* CraftWidget)
{
	check(MainCanvas);
	UCanvasPanelSlot* PanelSlot = MainCanvas->AddChildToCanvas(CraftWidget);
	check(PanelSlot);
	PanelSlot->SetSize(FVector2D(1000.f, 750.f));
	PanelSlot->SetPosition(FVector2D(50.f, 150.f));
}

void UHUDWidget::InitializeHUDWidget_Implementation()
{
}

void UHUDWidget::SetPlayerName_Implementation(const FText& NewName)
{
	check(PlayerName);
	PlayerName->SetText(NewName);
}

void UHUDWidget::SetPlayerLevel_Implementation(int32 NewLevel)
{
	check(PlayerLevel);
	PlayerLevel->SetText(FText::FromString(FString("Lv. ") + FString::FromInt(NewLevel)));
}

void UHUDWidget::SetPlayerHealth_Implementation(float CurrentHealth, float MaxHealth)
{
	check(StatusIndicatorWidget);
	StatusIndicatorWidget->UpdateHealthBar(MaxHealth, CurrentHealth);
}

void UHUDWidget::SetPlayerMana_Implementation(float CurrentMana, float MaxMana)
{
	check(StatusIndicatorWidget);
	StatusIndicatorWidget->UpdateManaBar(MaxMana, CurrentMana);
}

void UHUDWidget::SetPlayerStamina_Implementation(float CurrentStamina, float MaxStamina)
{
	check(StatusIndicatorWidget);
	StatusIndicatorWidget->UpdateStaminaBar(MaxStamina, CurrentStamina);
}
