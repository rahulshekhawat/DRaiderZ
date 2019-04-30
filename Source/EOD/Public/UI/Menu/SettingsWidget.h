// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidgetBase.h"
#include "SettingsWidget.generated.h"

class UButton;
class UTextBlock;
class UCanvasPanel;
class UWidgetSwitcher;
class UOptionsWidgetBase;
class UFooterButtonWidget;
class URegularButtonWidget;
class UActionConfirmationWidget;

/**
 * 
 */
UCLASS()
class EOD_API USettingsWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	USettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	URegularButtonWidget* GameButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	URegularButtonWidget* ControllerButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	URegularButtonWidget* MouseKeyboardButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	URegularButtonWidget* AudioButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	URegularButtonWidget* VideoButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	URegularButtonWidget* AdvancedButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* BackMainMenuButton;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UFooterButtonWidget* BackFooterButton;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UFooterButtonWidget* ResetDefaultsFooterButton;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* TipTextBlock;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* OptionsNameTextBlock;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UWidgetSwitcher* OptionsWidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UOptionsWidgetBase* GameOptionsMain;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UOptionsWidgetBase* AudioOptionsMain;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UOptionsWidgetBase* VideoOptionsMain;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UOptionsWidgetBase* AdvancedOptionsMain;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetWidgetSwitcherLayout(UWidget* NewActiveWidget, URegularButtonWidget* InCallingButton);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UnlockAllButtons();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SaveUserSettings();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void ReloadUserSettings();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void ExitSettings();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void ResetOptionWidgetsDirtyState();

protected:

	/** Returns true if a confirmation widget was added to screen */
	bool SwitchToConfirmationWidget();

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Classes")
	TSubclassOf<UActionConfirmationWidget> ConfirmationWidgetClass;

	// --------------------------------------
	//  Events
	// --------------------------------------

	UFUNCTION()
	void HandleGameButtonClicked();
	
	UFUNCTION()
	void HandleControllerButtonClicked();
	
	UFUNCTION()
	void HandleMouseKeyButtonClicked();
	
	UFUNCTION()
	void HandleAudioButtonClicked();
	
	UFUNCTION()
	void HandleVideoButtonClicked();
	
	UFUNCTION()
	void HandleAdvancedButtonClicked();

	UFUNCTION()
	void SaveUserSettingsAndExit();
	
	UFUNCTION()
	void ReloadUserSettingsAndExit();

};
