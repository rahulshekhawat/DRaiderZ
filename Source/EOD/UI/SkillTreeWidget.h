// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateTypes.h"
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
	USkillTreeItemContainer* SkillButton_Crush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Neutralize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_DodgeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_ArmaCrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Vanquish;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_UpwardStrike;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Nocturne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_BuffaloCharge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Bash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Tornado;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Scamper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Escape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_FuriousDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Outrage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Tranquility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeItemContainer* SkillButton_Madness;
	//~ End berserker skills


private:

	FORCEINLINE void SetupContainerPosition(USkillTreeItemContainer* Container);

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

	void ResetAllTabButtonsStyle();

	void ResetButtonStyle(UButton* Button);

	void SetButtonStyleToSelected(UButton* Button);
	


};
