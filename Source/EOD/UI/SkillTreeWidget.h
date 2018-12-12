// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Styling/SlateTypes.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class USkillTreeItemContainer;

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

	FSkillState GetSkillState(const FString& SkillGroup) const;

private:

	/** Move container to it's designated position */
	void SetupContainerPosition(USkillTreeItemContainer* Container);

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

	void ResetAllTabButtonsStyle();

	void ResetButtonStyle(UButton* Button);

	void SetButtonStyleToSelected(UButton* Button);
	
	FButtonStyle DefaultButtonStyle;

	USkillTreeItemContainer* GetSkillTreeSlotFromSkillGroupID(const FString& SkillGroupID);

};
