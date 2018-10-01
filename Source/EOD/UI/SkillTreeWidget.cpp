// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeWidget.h"

#include "Button.h"
#include "WidgetSwitcher.h"

USkillTreeWidget::USkillTreeWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeWidget::Initialize()
{
	if (Super::Initialize() &&
		AssassinTab &&
		BerserkerTab &&
		ClericTab &&
		DefenderTab &&
		SorcererTab &&
		SkillTreeSwitcher)
	{
		DefaultButtonStyle = AssassinTab->WidgetStyle;

		// Default open tab will be berserker tab
		SetButtonStyleToSelected(BerserkerTab);
		SkillTreeSwitcher->SetActiveWidgetIndex(1);

		// AssassinTab.AddDynamic()

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

void USkillTreeWidget::ResetAllTabButtonsStyle()
{
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
