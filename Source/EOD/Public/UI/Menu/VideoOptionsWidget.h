// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/OptionsWidgetBase.h"
#include "VideoOptionsWidget.generated.h"

class UScrollBox;
class UWidgetSwitcher;
class UGameUserSettings;
class UScrollButtonWidget;
class UResolutionSubWidget;
class UWindowModeSubWidget;
class UFrameRateSubWidget;
class UQualitySettingSubWidget;

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

	virtual void NativePreConstruct() override;

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
	UScrollButtonWidget* GraphicsQuality;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* VerticalSync;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* FrameRate;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Gamma;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UWindowModeSubWidget* WindowModeSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UResolutionSubWidget* ResolutionSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* GraphicsSub;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UFrameRateSubWidget* FrameRateSub;

	// --------------------------------------
	//  Widget Update
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentWindowMode(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentResolution(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentVerticalSync(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentFrameRate(UGameUserSettings* GameUserSettings = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentGraphicsQuality(UGameUserSettings* GameUserSettings = nullptr);

	virtual void InitializeOptions(bool bForceUserSettingReload = false) override;

	virtual void CloseDownOptions() override;

	virtual void ResetOptions() override;

protected:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UFUNCTION()
	void HandleWindowModeButtonClicked();

	UFUNCTION()
	void HandleResolutionButtonClicked();

	UFUNCTION()
	void HandleVSyncButtonClicked();

	UFUNCTION()
	void HandleFrameRateButtonClicked();
	
	UFUNCTION()
	void HandleGraphicsQualityButtonClicked();
	
	UFUNCTION()
	void HandleGraphicsQualitySelected(int32 SelectedQuality);


};
