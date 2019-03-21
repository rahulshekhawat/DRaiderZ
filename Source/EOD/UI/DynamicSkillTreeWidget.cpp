// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSkillTreeWidget.h"
#include "SkillPointsInfoWidget.h"
#include "EODPreprocessors.h"
#include "GameplaySkillBase.h"
#include "ContainerWidget.h"
#include "Components/SkillTreeComponent.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialInterface.h"

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

void UDynamicSkillTreeWidget::InitializeSkillTreeLayout(USkillTreeComponent* SkillTreeComponent, UDataTable* const SkillLayoutTable, const TMap<FName, FSkillTreeSlotSaveData>& SkillTreeSlotSaveData)
{
	InitializeSkillTreeLayout(SkillTreeComponent, SkillLayoutTable);

	TArray<FName> Keys;
	SkillContainersMap.GetKeys(Keys);

	for (FName Key : Keys)
	{
		if (SkillTreeSlotSaveData.Contains(Key))
		{
			UContainerWidget* Container = SkillContainersMap[Key];
			FSkillTreeSlotSaveData TempSlotData = SkillTreeSlotSaveData[Key];
			if (Container)
			{
				Container->SetCurrentValue(TempSlotData.CurrentUpgrade);
			}
		}
	}
}

void UDynamicSkillTreeWidget::UpdateSkillSlots()
{
	check(SkillTreeComp);
	TArray<FName> Keys;
	SkillContainersMap.GetKeys(Keys);

	if (SkillTreeComp->IsAnySkillPointAvailable())
	{
		UpdateSkillSlots_SkillPointsAvailable();
	}
	else
	{
		UpdateSkillSlots_NoSkillPointAvailable();
	}
}

UContainerWidget* UDynamicSkillTreeWidget::GetSkillSlotForSkillGroup(FName SkillGroup)
{
	if (SkillContainersMap.Contains(SkillGroup))
	{
		return SkillContainersMap[SkillGroup];
	}

	return nullptr;
}

void UDynamicSkillTreeWidget::InitializeSkillTreeLayout(USkillTreeComponent* SkillTreeComponent, UDataTable* SkillLayoutTable)
{
	if (!SkillLayoutTable || !SkillTreeComponent)
	{
		return;
	}

	SkillTreeComp = SkillTreeComponent;

	FString ContextString = FString("UDynamicSkillTreeWidget::InitializeSkillSlots()");
	TArray<FName> RowNames = SkillLayoutTable->GetRowNames();
	// Initialize skill slots
	for (FName RowName : RowNames)
	{
		FSkillTreeSlot* SkillTreeSlot = SkillLayoutTable->FindRow<FSkillTreeSlot>(RowName, ContextString);
		AddNewSkillSlot(RowName, SkillTreeSlot);

		// Draw connector arrow
		if (SkillTreeSlot->SkillRequiredToUnlock != NAME_None)
		{
			FSkillTreeSlot* UnlockSlot = SkillLayoutTable->FindRow<FSkillTreeSlot>(SkillTreeSlot->SkillRequiredToUnlock, ContextString);
			if (UnlockSlot)
			{
				UImage* TempImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
				if (TempImage)
				{
					FSlateBrush SlateBrush;
					SlateBrush.SetResourceObject(ArrowTexture);
					SlateBrush.DrawAs = ESlateBrushDrawType::Image;
					SlateBrush.Tiling = ESlateBrushTileType::NoTile;
					TempImage->SetBrush(SlateBrush);
				}
				SetupArrowPosition(TempImage, UnlockSlot->Vocation, UnlockSlot->ColumnPosition, UnlockSlot->RowPosition);
				ConnectorArrows.Add(TempImage);
			}
		}
	}
}

void UDynamicSkillTreeWidget::AddNewSkillSlot(FName SkillGroup, FSkillTreeSlot* SlotInfo)
{
	if (!SlotInfo || !SkillTreeSlotClass.Get())
	{
		return;
	}

	UContainerWidget* NewItemContainer = CreateWidget<UContainerWidget>(GetOwningPlayer(), SkillTreeSlotClass);
	if (NewItemContainer)
	{
		NewItemContainer->InitializeWithParent(this);

		UGameplaySkillBase* SkillObj = Cast<UGameplaySkillBase>(SlotInfo->PlayerSkill.Get()->GetDefaultObject());
		if (SkillObj)
		{
			NewItemContainer->SetIcon(SkillObj->GetSkillIcon());
			NewItemContainer->SetInGameName(SkillObj->GetInGameSkillName());
			NewItemContainer->SetDescription(SkillObj->GetInGameDescription());
		}
		NewItemContainer->SetMaxValue(SlotInfo->MaxUpgrades);
		NewItemContainer->SetItemID(SkillGroup);
		NewItemContainer->SetItemGroup(SkillGroup);
		
		if (SlotInfo->bUnlockedByDefault)
		{
			NewItemContainer->SetIsEnabled(true);
		}

		SetupSlotPosition(NewItemContainer, SlotInfo->Vocation, SlotInfo->ColumnPosition, SlotInfo->RowPosition);
		SkillContainersMap.Add(SkillGroup, NewItemContainer);

		NewItemContainer->OnClicked.AddDynamic(this, &UDynamicSkillTreeWidget::OnSkillSlotClicked);
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
	}
}

void UDynamicSkillTreeWidget::SetupArrowPosition(UImage* ArrowImage, EVocations Vocation, int32 ParentColumn, int32 ParentRow)
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

	int32 XPosition = 540 + ParentColumn * 180 + 5;
	int32 YPosition = 25 + ParentRow * 130 + 85;

	FVector2D Size(80.f, 50.f);

	UCanvasPanelSlot* CPSlot = CPanel->AddChildToCanvas(ArrowImage);
	if (CPSlot)
	{
		CPSlot->SetPosition(FVector2D(XPosition, YPosition));
		CPSlot->SetSize(Size);
	}
}

void UDynamicSkillTreeWidget::UpdateSkillSlots_NoSkillPointAvailable()
{
	
}

void UDynamicSkillTreeWidget::UpdateSkillSlots_SkillPointsAvailable()
{
	for (FName SkillGroup : Keys)
	{
		UContainerWidget* Wiget = SkillContainersMap[SkillGroup];
		// if (SkillTreeComp->Is)


		// If no skill point is allocated to this widget
		if (!SkillTreeComp->IsAnySkillPointAllocatedToSlot(SkillGroup))
		{
			// if ()
		}

		if (SkillTreeComp->IsAnySkillPointAllocatedToSlot(SkillGroup))
		{

		}
		// SkillTreeComp->Is
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

void UDynamicSkillTreeWidget::OnSkillSlotClicked(UContainerWidget* Widget, UUserWidget* ParentWidget)
{
	check(this == ParentWidget && Widget && SkillTreeComp);

	FContainerData ContData = Widget->GetContainerData();
	bool bAllocationSuccessful = SkillTreeComp->AttemptPointAllocationToSlot(ContData.ItemID);

	if (bAllocationSuccessful)
	{
		UpdateSkillSlots();
		UGameplayStatics::PlaySound2D(this, SkillPointAllocatedSound);
	}	
}
