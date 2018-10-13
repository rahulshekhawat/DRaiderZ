// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeWidget.h"
#include "SkillTreeItemContainer.h"

#include "Button.h"
#include "WidgetSwitcher.h"

USkillTreeWidget::USkillTreeWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeWidget::Initialize()
{
	if (!(Super::Initialize() &&
		AssassinTab &&
		BerserkerTab &&
		ClericTab &&
		DefenderTab &&
		SorcererTab &&
		SkillTreeSwitcher))
	{
		return false;
	}

	DefaultButtonStyle = AssassinTab->WidgetStyle;

	// Default open tab will be berserker tab
	SetButtonStyleToSelected(BerserkerTab);
	SkillTreeSwitcher->SetActiveWidgetIndex(1);

	AssassinTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateAssassinTab);
	BerserkerTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateBerserkerTab);
	ClericTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateClericTab);
	DefenderTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateDefenderTab);
	SorcererTab->OnClicked.AddDynamic(this, &USkillTreeWidget::ActivateSorcererTab);

	SetupContainerPosition(SkillButton_Crush);

	return true;
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FORCEINLINE void USkillTreeWidget::SetupContainerPosition(USkillTreeItemContainer* Container)
{
	/*
	switch (Container->ColumnPosition)
	{
	default:
		break;
	}
	*/

}

void USkillTreeWidget::ActivateAssassinTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(AssassinTab);
	SkillTreeSwitcher->SetActiveWidgetIndex(0);
}

void USkillTreeWidget::ActivateBerserkerTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(BerserkerTab);
	SkillTreeSwitcher->SetActiveWidgetIndex(1);
}

void USkillTreeWidget::ActivateClericTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(ClericTab);
	SkillTreeSwitcher->SetActiveWidgetIndex(2);
}

void USkillTreeWidget::ActivateDefenderTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(DefenderTab);
	SkillTreeSwitcher->SetActiveWidgetIndex(3);
}

void USkillTreeWidget::ActivateSorcererTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(SorcererTab);
	SkillTreeSwitcher->SetActiveWidgetIndex(4);
}

void USkillTreeWidget::ResetAllTabButtonsStyle()
{
	ResetButtonStyle(AssassinTab);
	ResetButtonStyle(BerserkerTab);
	ResetButtonStyle(ClericTab);
	ResetButtonStyle(DefenderTab);
	ResetButtonStyle(SorcererTab);
}

void USkillTreeWidget::ResetButtonStyle(UButton * Button)
{
	Button->SetStyle(DefaultButtonStyle);
}

void USkillTreeWidget::SetButtonStyleToSelected(UButton * Button)
{
	FButtonStyle ButtonStyle;
	ButtonStyle.SetNormal(DefaultButtonStyle.Pressed);
	ButtonStyle.SetPressed(DefaultButtonStyle.Pressed);
	ButtonStyle.SetHovered(DefaultButtonStyle.Pressed);

	Button->SetStyle(ButtonStyle);
}
