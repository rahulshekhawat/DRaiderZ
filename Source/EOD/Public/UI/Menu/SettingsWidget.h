// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidgetBase.h"
#include "SettingsWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UFooterButtonWidget;
class URegularButtonWidget;

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

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetWidgetSwitcherLayout(UWidget* NewActiveWidget, URegularButtonWidget* InCallingButton);



};
