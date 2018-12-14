// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeWidget.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/UI/SkillTreeItemContainer.h"
#include "EOD/Characters/PlayerCharacter.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanelSlot.h"

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
		SkillTreeSwitcher &&
		CrushSkillButton &&
		NeutralizeSkillButton &&
		DodgeAttackSkillButton &&
		ArmaCrushSkillButton &&
		VanquishSkillButton &&
		UpwardStrikeSkillButton &&
		NocturneSkillButton &&
		BuffaloChargeSkillButton &&
		BashSkillButton &&
		TornadoSkillButton &&
		ScamperSkillButton &&
		EscapeSkillButton &&
		FuriousDashSkillButton &&
		OutrageSkillButton &&
		TranquilitySkillButton &&
		MadnessSkillButton))
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

	SetupContainerPosition(CrushSkillButton);
	SetupContainerPosition(NeutralizeSkillButton);
	SetupContainerPosition(DodgeAttackSkillButton);
	SetupContainerPosition(ArmaCrushSkillButton);
	SetupContainerPosition(VanquishSkillButton);
	SetupContainerPosition(UpwardStrikeSkillButton);
	SetupContainerPosition(NocturneSkillButton);
	SetupContainerPosition(BuffaloChargeSkillButton);
	SetupContainerPosition(BashSkillButton);
	SetupContainerPosition(TornadoSkillButton);
	SetupContainerPosition(ScamperSkillButton);
	SetupContainerPosition(EscapeSkillButton);
	SetupContainerPosition(FuriousDashSkillButton);
	SetupContainerPosition(OutrageSkillButton);
	SetupContainerPosition(TranquilitySkillButton);
	SetupContainerPosition(MadnessSkillButton);

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

FSkillState USkillTreeWidget::GetSkillState(const FString & SkillGroup) const
{
	return FSkillState();
}

void USkillTreeWidget::SetupContainerPosition(USkillTreeItemContainer* Container)
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Container->Slot);

	if (!CanvasSlot)
	{
		return;
	}

	FVector2D Position;
	switch (Container->ColumnPosition)
	{
	case 0:
		Position.X = 540;
		break;
	case 1:
		Position.X = 720;
		break;
	case 2:
		Position.X = 900;
		break;
	case 3:
		Position.X = 1080;
		break;
	case 4:
		Position.X = 1260;
		break;
	default:
		break;
	}

	switch (Container->RowPosition)
	{
	case 0:
		Position.Y = 25;
		break;
	case 1:
		Position.Y = 155;
		break;
	case 2:
		Position.Y = 285;
		break;
	case 3:
		Position.Y = 415;
		break;
	case 4:
		Position.Y = 545;
		break;
	case 5:
		Position.Y = 675;
		break;
	case 6:
		Position.Y = 805;
		break;
	default:
		break;
	}

	CanvasSlot->SetPosition(Position);
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

USkillTreeItemContainer* USkillTreeWidget::GetSkillTreeSlotFromSkillGroupID(const FString & SkillGroupID)
{
	return nullptr;
}
