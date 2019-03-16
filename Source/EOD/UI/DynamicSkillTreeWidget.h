// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CharacterLibrary.h"
#include "Engine/DataTable.h"
#include "Components/Button.h"
#include "Styling/SlateTypes.h"
#include "Blueprint/UserWidget.h"
#include "DynamicSkillTreeWidget.generated.h"

class UButton;
class UCanvasPanel;
class UWidgetSwitcher;
class USkillPointsInfoWidget;
class USkillTreeItemContainer;

/**
 * 
 */
UCLASS()
class EOD_API UDynamicSkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UDynamicSkillTreeWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Child Widgets
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* AssassinCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* BerserkerCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* ClericCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* DefenderCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* SorcererCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* AssassinInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* BerserkerInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* ClericInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* DefenderInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* SorcererInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UWidgetSwitcher* SkillTreeSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* AssassinTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* BerserkerTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* ClericTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* DefenderTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	UButton* SorcererTab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Selection Tab", meta = (BindWidget))
	USkillPointsInfoWidget* SkillPointsInfo;

	// --------------------------------------
	//	Adding Skills Dynamically
	// --------------------------------------

	/** Initialize skill tree layout from SkillLayoutTable, i.e., create and add skill slots to the skill tree */
	void InitializeSkillTreeLayout(UDataTable* SkillLayoutTable);

	/** Initialize skill tree slots (add skill icon and information regarding current and maximum upgrades available) from PlayerSkillsMap */
	void InitializeSkillTreeSlots(const TMap<FName, UGameplaySkillBase*>& PlayerSkillsMap);

protected:

	/** The class to use for creating skill slot widgets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Class")
	TSubclassOf<USkillTreeItemContainer> SkillTreeSlotClass;

private:

	/** A map of skill group to it's respective skill slot container */
	UPROPERTY(Transient)
	TMap<FName, USkillTreeItemContainer*> SkillContainersMap;

	/** A list of all the assassin skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> AssassinSkills;

	/** A list of all the berserker skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> BerserkerSkills;

	/** A list of all the cleric skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> ClericSkills;

	/** A list of all the defender skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> DefenderSkills;

	/** A list of all the sorcerer skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> SorcererSkills;

	void AddNewSkillSlot(FName SkillGroup, FSkillTreeSlot* SlotInfo);

	void SetupSlotPosition(USkillTreeItemContainer* ItemContainer, EVocations Vocation, int32 Column, int32 Row);

	// --------------------------------------
	//	Tab Switching
	// --------------------------------------

	UPROPERTY()
	FButtonStyle DefaultButtonStyle;

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

	/** Index of tab currently selected/active in skill tree */
	int32 CurrentActiveTabIndex;
	
	void AddSkillPointsInfoToCanvas(UCanvasPanel* CPanel);


};

inline void UDynamicSkillTreeWidget::SetButtonStyleToSelected(UButton* Button)
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

inline void UDynamicSkillTreeWidget::ResetButtonStyle(UButton* Button)
{
	if (Button)
	{
		Button->SetStyle(DefaultButtonStyle);
	}
}

inline void UDynamicSkillTreeWidget::ResetTabStyleByIndex(int32 Index)
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
