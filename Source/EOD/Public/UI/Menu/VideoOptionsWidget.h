// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/OptionsWidgetBase.h"
#include "VideoOptionsWidget.generated.h"

class UScrollBox;
class UWidgetSwitcher;
class UGameUserSettings;
class UResolutionWidget;
class UScrollButtonWidget;

/**
 * 
 */
UCLASS()
class EOD_API UVideoOptionsWidget : public UOptionsWidgetBase
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UVideoOptionsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* WindowMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Resolution;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* VerticalSync;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* FrameRate;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Gamma;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UUserWidget* WindowModeSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UResolutionWidget* ResolutionSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UUserWidget* FrameRateSub;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollBox* VideoOptionsScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UWidgetSwitcher* SubWidgetSwitcher;

	// --------------------------------------
	//  Widget Initialization and Update
	// --------------------------------------

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	bool bIsInitialized;

	void InitializeOptions();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentWindowMode(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentResolution(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentVerticalSync(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentFrameRate(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void EnableScrollBoxItems();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void DisableScrollBoxItems(UWidget* ExcludedItem);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void MoveSubMenu(UWidget* InScrollItem);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void ToggleSubOptions(UWidget* SubWidget, UScrollButtonWidget* CallingScrollButton);

	virtual void CloseDownOptions() override;

	virtual void ResetOptions() override;

protected:

	// --------------------------------------
	//  Events
	// --------------------------------------

	void HandleResolutionButtonClicked();
	void HandleVSyncButtonClicked();


};
