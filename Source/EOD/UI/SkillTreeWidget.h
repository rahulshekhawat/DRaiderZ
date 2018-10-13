// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateTypes.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class USkillTreeContainer;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* SkillTreeSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* AssassinTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BerserkerTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClericTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* DefenderTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SorcererTab;

	//~ Begin berserker skills
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Crush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Neutralize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_DodgeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_ArmaCrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Vanquish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_UpwardStrike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Nocturne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_BuffaloCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Bash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Tornado;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Scamper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Escape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_FuriousDash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Outrage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Tranquility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeContainer* SkillButton_Madness;
	//~ End berserker skills


	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UEODItemContainer* VanquishSkill;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UEODItemContainer* VanquishSkill;

private:

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
