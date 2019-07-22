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

#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

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
		InteractivePopup)
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
