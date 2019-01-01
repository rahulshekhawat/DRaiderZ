// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/UI/SkillTreeItemContainer.h"

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

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	////////////////////////////////////////////////////////////////////////////////
	// Child Widgets
	////////////////////////////////////////////////////////////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* SkillTreeSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* AssassinTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* BerserkerTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ClericTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* DefenderTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SorcererTab;

	//~ Begin berserker skills
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* CrushSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* NeutralizeSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* DodgeAttackSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* ArmaCrushSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* VanquishSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* UpwardStrikeSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* NocturneSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* BuffaloChargeSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* BashSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* TornadoSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* ScamperSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* EscapeSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* FuriousDashSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* OutrageSkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* TranquilitySkillButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* MadnessSkillButton;
	//~ End berserker skills

	void RefreshVisuals();

	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
public:
	FSkillState GetSkillState(const FString& SkillGroup) const;

private:
	UPROPERTY()
	FButtonStyle DefaultButtonStyle;

	TArray<USkillTreeItemContainer*> BerserkerSkills;

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

	/** Initialize a skill tree container of type Berserker */
	inline void InitializeBerserkerSkillItemContainer(USkillTreeItemContainer* Container);

	/** Change visual style of given button to 'selected' */
	inline void SetButtonStyleToSelected(UButton* Button);

	/** Resets button style */
	inline void ResetButtonStyle(UButton* Button);

	/** Reset button style for all skill tree tab buttons */
	inline void ResetAllTabButtonsStyle();


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

inline void USkillTreeWidget::InitializeBerserkerSkillItemContainer(USkillTreeItemContainer* Container)
{
	SetupContainerPosition(Container);
	// @todo load skill icon
	// @todo set container to disabled state
	BerserkerSkills.Add(Container);
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
