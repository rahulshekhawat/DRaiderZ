// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeWidget.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/UI/SkillTreeItemContainer.h"
#include "EOD/Characters/PlayerCharacter.h"

#include "Components/WidgetSwitcher.h"

USkillTreeWidget::USkillTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

	InitializeBerserkerSkillItemContainer(CrushSkillButton);
	InitializeBerserkerSkillItemContainer(NeutralizeSkillButton);
	InitializeBerserkerSkillItemContainer(DodgeAttackSkillButton);
	InitializeBerserkerSkillItemContainer(ArmaCrushSkillButton);
	InitializeBerserkerSkillItemContainer(VanquishSkillButton);
	InitializeBerserkerSkillItemContainer(UpwardStrikeSkillButton);
	InitializeBerserkerSkillItemContainer(NocturneSkillButton);
	InitializeBerserkerSkillItemContainer(BuffaloChargeSkillButton);
	InitializeBerserkerSkillItemContainer(BashSkillButton);
	InitializeBerserkerSkillItemContainer(TornadoSkillButton);
	InitializeBerserkerSkillItemContainer(ScamperSkillButton);
	InitializeBerserkerSkillItemContainer(EscapeSkillButton);
	InitializeBerserkerSkillItemContainer(FuriousDashSkillButton);
	InitializeBerserkerSkillItemContainer(OutrageSkillButton);
	InitializeBerserkerSkillItemContainer(TranquilitySkillButton);
	InitializeBerserkerSkillItemContainer(MadnessSkillButton);

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

FSkillState USkillTreeWidget::GetSkillState(const FString& SkillGroup) const
{
	return FSkillState();
}

void USkillTreeWidget::ActivateAssassinTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(AssassinTab);
	if(IsValid(SkillTreeSwitcher))
	{
		SkillTreeSwitcher->SetActiveWidgetIndex(0);
	}
}

void USkillTreeWidget::ActivateBerserkerTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(BerserkerTab);
	if(IsValid(SkillTreeSwitcher))
	{
		SkillTreeSwitcher->SetActiveWidgetIndex(1);
	}
}

void USkillTreeWidget::ActivateClericTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(ClericTab);
	if(IsValid(SkillTreeSwitcher))
	{
		SkillTreeSwitcher->SetActiveWidgetIndex(2);
	}
}

void USkillTreeWidget::ActivateDefenderTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(DefenderTab);
	if(IsValid(SkillTreeSwitcher))
	{
		SkillTreeSwitcher->SetActiveWidgetIndex(3);
	}
}

void USkillTreeWidget::ActivateSorcererTab()
{
	ResetAllTabButtonsStyle();
	SetButtonStyleToSelected(SorcererTab);
	if(IsValid(SkillTreeSwitcher))
	{
		SkillTreeSwitcher->SetActiveWidgetIndex(4);
	}
}

USkillTreeItemContainer* USkillTreeWidget::GetSkillTreeSlotFromSkillGroupID(const FString& SkillGroupID)
{
	return nullptr;
}
