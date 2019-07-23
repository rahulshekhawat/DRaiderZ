// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeWidget.h"

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

void USkillTreeWidget::AddNewSTContainer(UPlayerSkillBase* PlayerSkill)
{
	//~ @todo
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

void USkillTreeWidget::OnSkillSlotClicked(USkillTreeContainerWidget* Widget, UUserWidget* ParentWidget)
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