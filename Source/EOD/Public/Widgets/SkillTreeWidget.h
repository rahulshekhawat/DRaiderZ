// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "SkillPointsInfoWidget.h"
#include "SkillTreeContainerWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanelSlot.h"

#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UPlayerSkillBase;
class USkillTreeComponent;

class USoundBase;

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillTreeWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
public:

	inline USkillPointsInfoWidget* GetSkillPointsInfoWidget() const { return SkillPointsInfo; }

protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* AssassinCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* BerserkerCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* ClericCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* DefenderCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* SorcererCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* AssassinInfoCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* BerserkerInfoCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* ClericInfoCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* DefenderInfoCanvas;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* SorcererInfoCanvas;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UWidgetSwitcher* SkillTreeSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* AssassinTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* BerserkerTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* ClericTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* DefenderTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* SorcererTab;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	USkillPointsInfoWidget* SkillPointsInfo;


	///////////////////////////////////////////////////////////////////////////
	//
public:

	/**
	 * Initialize skill tree layout from SkillLayoutTable, i.e., create and add skill slots to the skill tree
	 * @note Use this version to initialize skill tree if there is no player save game present (i.e., we don't have access to SkillTreeSlotSaveData)
	 */
	// void InitializeSkillTreeLayout(USkillTreeComponent* SkillTreeComponent, UDataTable* SkillLayoutTable);

	/**
	 * Initialize skill tree layout from SkillLayoutTable, i.e., create and add skill slots to the skill tree
	 * Update skill tree slot information from SkillTreeSlotSaveData
	 */
	// void InitializeSkillTreeLayout(USkillTreeComponent* SkillTreeComponent, UDataTable* const SkillLayoutTable, const TMap<FName, FSkillTreeSlotSaveData>& SkillTreeSlotSaveData);

	/** Get skill slot associated with with the SkillGroup */
	// UContainerWidget* GetSkillSlotForSkillGroup(FName SkillGroup);

	/** Iterates over all skill slots in this tree and updates the bIsEnabled, bCanBeDragged, bCanBeClicked state of skill slot */
	// void UpdateSkillSlots();


protected:

	/** The skill tree component responsible for managing this skill tree widget */
	// UPROPERTY(Transient)
	// USkillTreeComponent* SkillTreeComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* SkillPointAllocatedSound;

	/** The class to use for creating skill slot widgets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillTreeContainerWidget> SkillTreeSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UMaterialInterface* ArrowTexture;

private:

	/** Add a new skill tree container to skill tree from player skill */
	void AddNewSTContainer(UPlayerSkillBase* PlayerSkill);

	inline void SetupSlotPosition(USkillTreeContainerWidget* ItemContainer, EVocations Vocation, int32 Column, int32 Row);

	inline void SetupArrowPosition(UImage* ArrowImage, EVocations Vocation, int32 ParentColumn, int32 ParentRow);

	/** Images of an arrow linking skill slots */
	UPROPERTY(Transient)
	TArray<UImage*> ConnectorArrows;

	/** A map of skill group to it's respective skill slot container */
	UPROPERTY(Transient)
	TMap<FName, USkillTreeContainerWidget*> SkillContainersMap;


	///////////////////////////////////////////////////////////////////////////
	//  Tab Switching
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

	/** Change visual style of given button to 'selected' */
	inline void SetButtonStyleToSelected(UButton* Button);

	/** Resets button style */
	inline void ResetButtonStyle(UButton* Button);

	inline void ResetTabStyleByIndex(int32 Index);

	inline void AddSkillPointsInfoToCanvas(UCanvasPanel* CPanel);

	inline UCanvasPanel* GetCanvasPanelOfVocation(EVocations Vocation);
	
	UFUNCTION()
	void OnSkillSlotClicked(class USkillTreeContainerWidget* Widget, UUserWidget* ParentWidget);

	FButtonStyle DefaultButtonStyle;

	/** Index of tab currently selected/active in skill tree */
	int32 CurrentActiveTabIndex;

};

inline void USkillTreeWidget::SetupSlotPosition(USkillTreeContainerWidget* ItemContainer, EVocations Vocation, int32 Column, int32 Row)
{
	UCanvasPanel* CPanel = GetCanvasPanelOfVocation(Vocation);
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

inline void USkillTreeWidget::SetupArrowPosition(UImage* ArrowImage, EVocations Vocation, int32 ParentColumn, int32 ParentRow)
{
	UCanvasPanel* CPanel = GetCanvasPanelOfVocation(Vocation);
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

inline void USkillTreeWidget::SetButtonStyleToSelected(UButton* Button)
{
	if (Button)
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
	if (Button)
	{
		Button->SetStyle(DefaultButtonStyle);
	}
}

inline void USkillTreeWidget::ResetTabStyleByIndex(int32 Index)
{
	switch (Index)
	{
	case 0:
		ResetButtonStyle(AssassinTab);
	case 1:
		ResetButtonStyle(BerserkerTab);
	case 2:
		ResetButtonStyle(ClericTab);
	case 3:
		ResetButtonStyle(DefenderTab);
	case 4:
		ResetButtonStyle(SorcererTab);
	default:
		break;
	}
}

inline void USkillTreeWidget::AddSkillPointsInfoToCanvas(UCanvasPanel* CPanel)
{
	if (CPanel && SkillPointsInfo)
	{
		UCanvasPanelSlot* CPSlot = CPanel->AddChildToCanvas(SkillPointsInfo);
		CPSlot->SetSize(FVector2D(440.f, 320.f));
		CPSlot->SetPosition(FVector2D(0.f, -25.f));
		CPSlot->SetAnchors(FAnchors(0.5f, 1.f, 0.5f, 1.f));
		CPSlot->SetAlignment(FVector2D(0.5f, 1.f));
	}
}

inline UCanvasPanel* USkillTreeWidget::GetCanvasPanelOfVocation(EVocations Vocation)
{
	switch (Vocation)
	{
	case EVocations::Assassin:
		return AssassinCanvas;
	case EVocations::Berserker:
		return BerserkerCanvas;
	case EVocations::Cleric:
		return ClericCanvas;
	case EVocations::Defender:
		return DefenderCanvas;
	case EVocations::Sorcerer:
		return SorcererCanvas;
	default:
		break;
	}
	return nullptr;
}
