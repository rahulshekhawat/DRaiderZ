// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateTypes.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UButton;
class UWidgetSwitcher;
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
	UButton* AssassinTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BerserkerTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ClericTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* DefenderTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SorcererTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* SkillTreeSwitcher;

	FButtonStyle DefaultButtonStyle;

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

	void ResetAllTabButtonsStyle();

	void ResetButtonStyle(UButton* Button);

	void SetButtonStyleToSelected(UButton* Button);
	


};
