// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "SkillTreeItemContainer.h"

#include "Components/Button.h"
#include "Styling/SlateTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "SkillTreeWidget.generated.h"

class UButton;
class UWidgetSwitcher;
// class USkillTreeItemContainer;

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USkillTreeWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	////////////////////////////////////////////////////////////////////////////////
	// Child Widgets
	////////////////////////////////////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree", meta = (BindWidget))
	UWidgetSwitcher* SkillTreeSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Tab", meta = (BindWidget))
	UButton* AssassinTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Tab", meta = (BindWidget))
	UButton* BerserkerTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Tab", meta = (BindWidget))
	UButton* ClericTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Tab", meta = (BindWidget))
	UButton* DefenderTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Tab", meta = (BindWidget))
	UButton* SorcererTab;

	//~ Begin Assassin Skills
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* ChaosStingerSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* StormBladesSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* RisingDragonSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* CleaveSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* CycloneKickSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* FatalStabSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* ShadowWalkSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* DragonicLeapSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* VeilOfDarknessSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* PenetrationStingerSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* WhirlingBladeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* RestorativeElixirSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* ShadowStrikeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* ShadowDanceSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Assassin Skills", meta = (BindWidget))
	USkillTreeItemContainer* BladeDanceSkillButton;
	//~ End Assassin Skills

	//~ Begin Berserker Skills
	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* CrushSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* NeutralizeSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* DodgeAttackSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* ArmaCrushSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* VanquishSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* UpwardStrikeSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* NocturneSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* BuffaloChargeSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* BashSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* TornadoSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* ScamperSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* EscapeSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* FuriousDashSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* OutrageSkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* TranquilitySkillButton;

	UPROPERTY(BlueprintReadOnly, Category = "Berserker Skills", meta = (BindWidget))
	USkillTreeItemContainer* MadnessSkillButton;
	//~ End Berserker Skills

	//~ Begin Defender Skills
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* StunningSmashSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* ShieldSlamSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* StrikeOfRuinSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* RushSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* RapidAssaultSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* SlamSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* PiercingStrikeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* ChaoticStrikeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* CycloneStrikeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* ComebackSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* BastionSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* VigilanceSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* EvasiveStrikeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* RetributionSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* MassiveStrikeSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* StoneArmorSkillButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defender Skills", meta = (BindWidget))
	USkillTreeItemContainer* ResilianceSkillButton;
	//~ End Defender Skills

	void RefreshVisuals();

	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
public:
	FSkillState GetSkillState(const FString& SkillGroup) const;

private:
	UPROPERTY()
	FButtonStyle DefaultButtonStyle;

	TArray<USkillTreeItemContainer*> AssassinSkills;

	TArray<USkillTreeItemContainer*> BerserkerSkills;

	TArray<USkillTreeItemContainer*> ClericSkills;

	TArray<USkillTreeItemContainer*> DefenderSkills;

	TArray<USkillTreeItemContainer*> SorcererSkills;

private:
	UFUNCTION()
	void ActivateAssassinTab();

	UFUNCTION()
	void ActivateBerserkerTab();

	UFUNCTION()
	void ActivateClericTab();

	UFUNCTION()
	void ActivateDefenderTab();

	UFUNCTION()
	void ActivateSorcererTab();

	USkillTreeItemContainer* GetSkillTreeSlotFromSkillGroupID(const FString& SkillGroupID);

	/** Get the desired position of container on skill tree widget based on container's row and column */
	inline FVector2D GetContainerDesiredPosition(int32 RowPosition, int32 ColumnPosition) const;

	/** Move container to it's designated position */
	inline void SetupContainerPosition(USkillTreeItemContainer* Container);

	/** Initialize a skill tree container of type Assassin */
	inline void InitializeAssassinSkillItemContainer(USkillTreeItemContainer* Container);

	/** Initialize a skill tree container of type Berserker */
	inline void InitializeBerserkerSkillItemContainer(USkillTreeItemContainer* Container);

	/** Initialize a skill tree container of type Defender */
	inline void InitializeDefenderSkillItemContainer(USkillTreeItemContainer* Container);

	/** Change visual style of given button to 'selected' */
	inline void SetButtonStyleToSelected(UButton* Button);

	/** Resets button style */
	inline void ResetButtonStyle(UButton* Button);

	/** Reset button style for all skill tree tab buttons */
	inline void ResetAllTabButtonsStyle();

	/** Load skill tree from a previous savegame */
	void LoadSkillTreeState();

};

inline FVector2D USkillTreeWidget::GetContainerDesiredPosition(int32 RowPosition, int32 ColumnPosition) const
{
	FVector2D Position;
	Position.X = 540 + 180 * ColumnPosition;
	Position.Y = 25 + 130 * RowPosition;
	return Position;
}

inline void USkillTreeWidget::SetupContainerPosition(USkillTreeItemContainer* Container)
{
	UCanvasPanelSlot* CanvasSlot = IsValid(Container) ? Cast<UCanvasPanelSlot>(Container->Slot) : nullptr;
	if (IsValid(CanvasSlot) && IsValid(Container))
	{
		FVector2D DesiredPos = GetContainerDesiredPosition(Container->RowPosition, Container->ColumnPosition);
		CanvasSlot->SetPosition(DesiredPos);
	}
}

inline void USkillTreeWidget::InitializeAssassinSkillItemContainer(USkillTreeItemContainer* Container)
{
	if (IsValid(Container))
	{
		Container->SetParentWidget(this);
		SetupContainerPosition(Container);
		AssassinSkills.Add(Container);
	}
}

inline void USkillTreeWidget::InitializeBerserkerSkillItemContainer(USkillTreeItemContainer* Container)
{
	if (IsValid(Container))
	{
		Container->SetParentWidget(this);
		SetupContainerPosition(Container);
		BerserkerSkills.Add(Container);
	}
}

inline void USkillTreeWidget::InitializeDefenderSkillItemContainer(USkillTreeItemContainer* Container)
{
	if (IsValid(Container))
	{
		Container->SetParentWidget(this);
		SetupContainerPosition(Container);
		DefenderSkills.Add(Container);
	}
}

inline void USkillTreeWidget::SetButtonStyleToSelected(UButton* Button)
{
	if (IsValid(Button))
	{
		FButtonStyle ButtonStyle;
		ButtonStyle.SetNormal(DefaultButtonStyle.Pressed);
		ButtonStyle.SetPressed(DefaultButtonStyle.Pressed);
		ButtonStyle.SetHovered(DefaultButtonStyle.Pressed);

		Button->SetStyle(ButtonStyle);
	}
}

inline void USkillTreeWidget::ResetButtonStyle(UButton* Button)
{
	if (IsValid(Button))
	{
		Button->SetStyle(DefaultButtonStyle);
	}
}

inline void USkillTreeWidget::ResetAllTabButtonsStyle()
{
	ResetButtonStyle(AssassinTab);
	ResetButtonStyle(BerserkerTab);
	ResetButtonStyle(ClericTab);
	ResetButtonStyle(DefenderTab);
	ResetButtonStyle(SorcererTab);
}
