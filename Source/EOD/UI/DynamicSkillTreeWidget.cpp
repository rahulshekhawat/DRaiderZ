// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSkillTreeWidget.h"
#include "SkillPointsInfoWidget.h"
#include "EODPreprocessors.h"
#include "GameplaySkillBase.h"
#include "ContainerWidget.h"

#include "Button.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanelSlot.h"

UDynamicSkillTreeWidget::UDynamicSkillTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDynamicSkillTreeWidget::Initialize()
{
	if (Super::Initialize() &&
		AssassinCanvas &&
		BerserkerCanvas &&
		ClericCanvas &&
		DefenderCanvas &&
		SorcererCanvas &&
		AssassinInfoCanvas &&
		BerserkerInfoCanvas &&
		ClericInfoCanvas &&
		DefenderInfoCanvas &&
		SorcererInfoCanvas &&
		SkillTreeSwitcher &&
		AssassinTab &&
		BerserkerTab &&
		ClericTab &&
		DefenderTab &&
		SorcererTab &&
		SkillTreeSwitcher)
	{
		DefaultButtonStyle = AssassinTab->WidgetStyle;

		AssassinTab->OnClicked.AddDynamic(this, &UDynamicSkillTreeWidget::ActivateAssassinTab);
		BerserkerTab->OnClicked.AddDynamic(this, &UDynamicSkillTreeWidget::ActivateBerserkerTab);
		ClericTab->OnClicked.AddDynamic(this, &UDynamicSkillTreeWidget::ActivateClericTab);
		DefenderTab->OnClicked.AddDynamic(this, &UDynamicSkillTreeWidget::ActivateDefenderTab);
		SorcererTab->OnClicked.AddDynamic(this, &UDynamicSkillTreeWidget::ActivateSorcererTab);

		// Berserker tab will open by default
		ActivateBerserkerTab();

		return true;
	}

	return false;
}

void UDynamicSkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDynamicSkillTreeWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDynamicSkillTreeWidget::InitializeSkillTreeLayout(UDataTable* const SkillLayoutTable, const TMap<FName, FSkillTreeSlotSaveData>& SkillTreeSlotSaveData)
{
	InitializeSkillTreeLayout(SkillLayoutTable);

	// @todo skillTreeSlotSaveData load
}

void UDynamicSkillTreeWidget::InitializeSkillTreeLayout(UDataTable* SkillLayoutTable)
{
	if (!SkillLayoutTable)
	{
		return;
	}

	FString ContextString = FString("UDynamicSkillTreeWidget::InitializeSkillSlots()");
	TArray<FName> RowNames = SkillLayoutTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FSkillTreeSlot* SkillTreeSlot = SkillLayoutTable->FindRow<FSkillTreeSlot>(RowName, ContextString);
		AddNewSkillSlot(RowName, SkillTreeSlot);
	}
}

/*
void UDynamicSkillTreeWidget::InitializeSkillTreeSlots(const TMap<FName, UGameplaySkillBase*>& PlayerSkillsMap)
{
	TArray<FName> Keys;
	PlayerSkillsMap.GetKeys(Keys);
	for (FName Key : Keys)
	{
		UGameplaySkillBase* Skill = PlayerSkillsMap[Key];
		if (SkillContainersMap.Contains(Key) && Skill)
		{
			UContainerWidget* Container = SkillContainersMap[Key];
			if (Container)
			{
				Container->EODItemInfo.InGameName = Skill->GetInGameSkillName();
				Container->EODItemInfo.Description = Skill->GetInGameDescription();
				Container->EODItemInfo.EODItemType = EEODItemType::ActiveSkill;
				Container->EODItemInfo.Icon = Skill->GetSkillIcon();
				Container->EODItemInfo.ItemGroup = Key.ToString();
				Container->SkillGroup = Key.ToString();
				Container->SkillState.CurrentUpgradeLevel = Skill->GetCurrentUpgrade();
				Container->SkillState.MaxUpgradeLevel = Skill->GetMaxUpgradeLevel();
				Container->RefreshContainerVisuals();
			}
		}
	}
}
*/

void UDynamicSkillTreeWidget::AddNewSkillSlot(FName SkillGroup, FSkillTreeSlot* SlotInfo)
{
	if (!SlotInfo || !SkillTreeSlotClass.Get())
	{
		return;
	}

	UContainerWidget* NewItemContainer = CreateWidget<UContainerWidget>(GetOwningPlayer(), SkillTreeSlotClass);
	if (NewItemContainer)
	{
		SetupSlotPosition(NewItemContainer, SlotInfo->Vocation, SlotInfo->ColumnPosition, SlotInfo->RowPosition);
		SkillContainersMap.Add(SkillGroup, NewItemContainer);
		UGameplaySkillBase* SkillObj = Cast<UGameplaySkillBase>(SlotInfo->PlayerSkill.Get()->GetDefaultObject());
		if (SkillObj)
		{
			NewItemContainer->SetIcon(SkillObj->GetSkillIcon());
		}

		
	}
}

void UDynamicSkillTreeWidget::SetupSlotPosition(UContainerWidget* ItemContainer, EVocations Vocation, int32 Column, int32 Row)
{
	UCanvasPanel* CPanel = nullptr;

	switch (Vocation)
	{
	case EVocations::Assassin:
		CPanel = AssassinCanvas;
		break;
	case EVocations::Berserker:
		CPanel = BerserkerCanvas;
		break;
	case EVocations::Cleric:
		CPanel = ClericCanvas;
		break;
	case EVocations::Defender:
		CPanel = DefenderCanvas;
		break;
	case EVocations::Sorcerer:
		CPanel = SorcererCanvas;
		break;
	default:
		break;
	}

	if (!CPanel)
	{
		return;
	}

	int32 XPosition = 540 + Column * 180;
	int32 YPosition = 25 + Row * 130;

	FVector2D Size(90.f, 90.f);

	UCanvasPanelSlot* CPSlot = CPanel->AddChildToCanvas(ItemContainer);
	if (CPSlot)
	{
		CPSlot->SetPosition(FVector2D(XPosition, YPosition));
		CPSlot->SetSize(Size);
		//~ @todo
		// ItemContainer->RefreshContainerVisuals();
	}
}

void UDynamicSkillTreeWidget::ActivateAssassinTab()
{
	if (CurrentActiveTabIndex != 0 && SkillTreeSwitcher)
	{
		ResetTabStyleByIndex(CurrentActiveTabIndex);
		SetButtonStyleToSelected(AssassinTab);
		SkillTreeSwitcher->SetActiveWidgetIndex(0);
		CurrentActiveTabIndex = 0;

		AddSkillPointsInfoToCanvas(AssassinInfoCanvas);
	}
}

void UDynamicSkillTreeWidget::ActivateBerserkerTab()
{
	if (CurrentActiveTabIndex != 1 && SkillTreeSwitcher)
	{
		ResetTabStyleByIndex(CurrentActiveTabIndex);
		SetButtonStyleToSelected(BerserkerTab);
		SkillTreeSwitcher->SetActiveWidgetIndex(1);
		CurrentActiveTabIndex = 1;

		AddSkillPointsInfoToCanvas(BerserkerInfoCanvas);
	}
}

void UDynamicSkillTreeWidget::ActivateClericTab()
{
	if (CurrentActiveTabIndex != 2 && SkillTreeSwitcher)
	{
		ResetTabStyleByIndex(CurrentActiveTabIndex);
		SetButtonStyleToSelected(ClericTab);
		SkillTreeSwitcher->SetActiveWidgetIndex(2);
		CurrentActiveTabIndex = 2;

		AddSkillPointsInfoToCanvas(ClericInfoCanvas);
	}
}

void UDynamicSkillTreeWidget::ActivateDefenderTab()
{
	if (CurrentActiveTabIndex != 3 && SkillTreeSwitcher)
	{
		ResetTabStyleByIndex(CurrentActiveTabIndex);
		SetButtonStyleToSelected(DefenderTab);
		SkillTreeSwitcher->SetActiveWidgetIndex(3);
		CurrentActiveTabIndex = 3;

		AddSkillPointsInfoToCanvas(DefenderInfoCanvas);
	}
}

void UDynamicSkillTreeWidget::ActivateSorcererTab()
{
	if (CurrentActiveTabIndex != 4 && SkillTreeSwitcher)
	{
		ResetTabStyleByIndex(CurrentActiveTabIndex);
		SetButtonStyleToSelected(SorcererTab);
		SkillTreeSwitcher->SetActiveWidgetIndex(4);
		CurrentActiveTabIndex = 4;

		AddSkillPointsInfoToCanvas(SorcererInfoCanvas);
	}
}

void UDynamicSkillTreeWidget::AddSkillPointsInfoToCanvas(UCanvasPanel* CPanel)
{
	if (CPanel && SkillPointsInfo)
	{
		UCanvasPanelSlot* CPSlot = CPanel->AddChildToCanvas(SkillPointsInfo);
		CPSlot->SetSize(FVector2D(400.f, 320.f));
		CPSlot->SetPosition(FVector2D(0.f, -25.f));
		CPSlot->SetAnchors(FAnchors(0.5f, 1.f, 0.5f, 1.f));
		CPSlot->SetAlignment(FVector2D(0.5f, 1.f));
	}
}