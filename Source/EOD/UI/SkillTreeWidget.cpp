// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeWidget.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/EODGameInstance.h"
#include "EOD/UI/SkillTreeItemContainer.h"
#include "EOD/Characters/PlayerCharacter.h"

#include "Components/WidgetSwitcher.h"

USkillTreeWidget::USkillTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeWidget::Initialize()
{
	if (!(Super::Initialize() &&
		SkillTreeSwitcher &&
		AssassinTab &&
		BerserkerTab &&
		ClericTab &&
		DefenderTab &&
		SorcererTab &&
		ChaosStingerSkillButton &&
		StormBladesSkillButton &&
		RisingDragonSkillButton &&
		CleaveSkillButton &&
		CycloneKickSkillButton &&
		FatalStabSkillButton &&
		ShadowWalkSkillButton &&
		DragonicLeapSkillButton &&
		VeilOfDarknessSkillButton &&
		PenetrationStingerSkillButton &&
		WhirlingBladeSkillButton &&
		RestorativeElixirSkillButton &&
		ShadowStrikeSkillButton &&
		ShadowDanceSkillButton &&
		BladeDanceSkillButton &&
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
		MadnessSkillButton &&
		StunningSmashSkillButton &&
		ShieldSlamSkillButton &&
		StrikeOfRuinSkillButton &&
		RushSkillButton &&
		RapidAssaultSkillButton &&
		SlamSkillButton &&
		PiercingStrikeSkillButton &&
		ChaoticStrikeSkillButton &&
		CycloneStrikeSkillButton &&
		ComebackSkillButton &&
		BastionSkillButton &&
		VigilanceSkillButton &&
		EvasiveStrikeSkillButton &&
		RetributionSkillButton &&
		MassiveStrikeSkillButton &&
		StoneArmorSkillButton &&
		ResilianceSkillButton))
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

	InitializeAssassinSkillItemContainer(ChaosStingerSkillButton);
	InitializeAssassinSkillItemContainer(StormBladesSkillButton);
	InitializeAssassinSkillItemContainer(RisingDragonSkillButton);
	InitializeAssassinSkillItemContainer(CleaveSkillButton);
	InitializeAssassinSkillItemContainer(CycloneKickSkillButton);
	InitializeAssassinSkillItemContainer(FatalStabSkillButton);
	InitializeAssassinSkillItemContainer(ShadowWalkSkillButton);
	InitializeAssassinSkillItemContainer(DragonicLeapSkillButton);
	InitializeAssassinSkillItemContainer(VeilOfDarknessSkillButton);
	InitializeAssassinSkillItemContainer(PenetrationStingerSkillButton);
	InitializeAssassinSkillItemContainer(WhirlingBladeSkillButton);
	InitializeAssassinSkillItemContainer(RestorativeElixirSkillButton);
	InitializeAssassinSkillItemContainer(ShadowStrikeSkillButton);
	InitializeAssassinSkillItemContainer(ShadowDanceSkillButton);
	InitializeAssassinSkillItemContainer(BladeDanceSkillButton);

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
	
	InitializeDefenderSkillItemContainer(StunningSmashSkillButton);
	InitializeDefenderSkillItemContainer(ShieldSlamSkillButton);
	InitializeDefenderSkillItemContainer(StrikeOfRuinSkillButton);
	InitializeDefenderSkillItemContainer(RushSkillButton);
	InitializeDefenderSkillItemContainer(RapidAssaultSkillButton);
	InitializeDefenderSkillItemContainer(SlamSkillButton);
	InitializeDefenderSkillItemContainer(PiercingStrikeSkillButton);
	InitializeDefenderSkillItemContainer(ChaoticStrikeSkillButton);
	InitializeDefenderSkillItemContainer(CycloneStrikeSkillButton);
	InitializeDefenderSkillItemContainer(ComebackSkillButton);
	InitializeDefenderSkillItemContainer(BastionSkillButton);
	InitializeDefenderSkillItemContainer(VigilanceSkillButton);
	InitializeDefenderSkillItemContainer(EvasiveStrikeSkillButton);
	InitializeDefenderSkillItemContainer(RetributionSkillButton);
	InitializeDefenderSkillItemContainer(MassiveStrikeSkillButton);
	InitializeDefenderSkillItemContainer(StoneArmorSkillButton);
	InitializeDefenderSkillItemContainer(ResilianceSkillButton);

	LoadSkillTreeState();

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

void USkillTreeWidget::RefreshVisuals()
{
	for (USkillTreeItemContainer* Cont : BerserkerSkills)
	{
		if (IsValid(Cont))
		{
			Cont->RefreshContainerVisuals();
		}
	}
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

void USkillTreeWidget::LoadSkillTreeState()
{
	/*
	UEODGameInstance* GI = Cast<UEODGameInstance>(GetGameInstance());
	if (!IsValid(GI))
	{
		return;
	}

	for (USkillTreeItemContainer* Container : AssassinSkills)
	{
		if (GI->SGToSSMap.Contains(Container->SkillGroup))
		{
			// Container->SkillState = GI->SGToSSMap[Container->SkillGroup];
			// Container->();
		}
	}
	*/
}
