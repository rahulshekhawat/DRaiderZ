// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeWidget.h"
#include "PlayerSkillBase.h"
#include "PlayerSkillsComponent.h"

#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialInterface.h"

USkillTreeWidget::USkillTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeWidget::Initialize()
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

		AssassinTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateAssassinTab);
		BerserkerTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateBerserkerTab);
		ClericTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateClericTab);
		DefenderTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateDefenderTab);
		SorcererTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateSorcererTab);

		// Berserker tab will open by default
		ActivateBerserkerTab();

		return true;
	}

	return false;
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillTreeWidget::InitializeSkillTreeLayout(UDataTable* STLayoutTable, UPlayerSkillsComponent* InSkillsComp)
{
	ensureMsgf(STLayoutTable != nullptr, TEXT("Tried to initialize skill tree with an invalid STLayoutTable"));
	if (STLayoutTable == nullptr)
	{
		return;
	}

	check(InSkillsComp);
	OwnerSkillsComp = InSkillsComp;

	FString ContextString = TEXT(__FUNCTION__);
	TArray<FName> RowNames = STLayoutTable->GetRowNames();

	const TMap<FName, UGameplaySkillBase*>& SGToSkillMap =  InSkillsComp->GetSkillGroupToSkillMap();

	for (FName RowName : RowNames)
	{
		// Just in case InitializeSkillTreeLayout has already been called
		if (SkillContainersMap.Contains(RowName))
		{
			continue;
		}

		FSkillTreeSlot* SkillTreeSlot = STLayoutTable->FindRow<FSkillTreeSlot>(RowName, ContextString);
		check(SkillTreeSlot);
		
		UPlayerSkillBase* PlayerSkill = SGToSkillMap.Contains(RowName) ? Cast<UPlayerSkillBase>(SGToSkillMap[RowName]) : nullptr;
		USkillTreeContainerWidget* STWidget = AddNewSTContainer(PlayerSkill);
		if (STWidget == nullptr)
		{
			continue;
		}

		SetupSlotPosition(STWidget, SkillTreeSlot->Vocation, SkillTreeSlot->ColumnPosition, SkillTreeSlot->RowPosition);

		if (SkillTreeSlot->SkillRequiredToUnlock != NAME_None)
		{
			FSkillTreeSlot* UnlockSlot = STLayoutTable->FindRow<FSkillTreeSlot>(SkillTreeSlot->SkillRequiredToUnlock, ContextString);
			check(UnlockSlot);

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

USkillTreeContainerWidget* USkillTreeWidget::AddNewSTContainer(UPlayerSkillBase* PlayerSkill)
{
	if (PlayerSkill == nullptr || SkillTreeSlotClass.Get() == nullptr || SkillContainersMap.Contains(PlayerSkill->GetSkillGroup()))
	{
		return nullptr;
	}

	USkillTreeContainerWidget* STContainer = CreateWidget<USkillTreeContainerWidget>(GetOwningPlayer(), SkillTreeSlotClass);
	if (STContainer)
	{
		STContainer->SetDataObj(PlayerSkill);
		STContainer->OnClicked.AddUniqueDynamic(this, &USkillTreeWidget::OnSkillSlotClicked);
		SkillContainersMap.Add(PlayerSkill->GetSkillGroup(), STContainer);
	}

	return STContainer;
}

void USkillTreeWidget::OnSkillSlotClicked(UContainerWidgetBase* Widget)
{
	//~ @todo
	/*
	check(this == ParentWidget && Widget && SkillTreeComp);

	FContainerData ContData = Widget->GetContainerData();
	bool bAllocationSuccessful = SkillTreeComp->AttemptPointAllocationToSlot(ContData.ItemID);

	if (bAllocationSuccessful)
	{
		UpdateSkillSlots();
		UGameplayStatics::PlaySound2D(this, SkillPointAllocatedSound);
	}
	*/
}

void USkillTreeWidget::ActivateAssassinTab()
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

void USkillTreeWidget::ActivateBerserkerTab()
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

void USkillTreeWidget::ActivateClericTab()
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

void USkillTreeWidget::ActivateDefenderTab()
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

void USkillTreeWidget::ActivateSorcererTab()
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
