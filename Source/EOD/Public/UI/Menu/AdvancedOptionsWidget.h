// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/OptionsWidgetBase.h"
#include "AdvancedOptionsWidget.generated.h"

class UWidgetSwitcher;
class UGameUserSettings;
class UScrollButtonWidget;
class UQualitySettingSubWidget;

/**
 * 
 */
UCLASS()
class EOD_API UAdvancedOptionsWidget : public UOptionsWidgetBase
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UAdvancedOptionsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* AntiAliasing;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* ViewDistance;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Effects;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Lighting;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Textures;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* PostProcessing;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Foliage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* ResolutionScale;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* AntiAliasingSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* ViewDistanceSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* EffectsSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* LightingSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* TexturesSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* PostProcessingSub;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* FoliageSub;

	// --------------------------------------
	//  Widget Update
	// --------------------------------------

	virtual void InitializeOptions(bool bForceUserSettingReload = false) override;
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentAntiAliasing(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentViewDistance(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentEffects(UGameUserSettings* GameUserSettings = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentLighting(UGameUserSettings* GameUserSettings = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentTextures(UGameUserSettings* GameUserSettings = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentPostProcessing(UGameUserSettings* GameUserSettings = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentFoliage(UGameUserSettings* GameUserSettings = nullptr);

protected:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UFUNCTION()
	void HandleAntiAliasingButtonClicked();

	UFUNCTION()
	void HandleViewDistanceButtonClicked();
	
	UFUNCTION()
	void HandleEffectsButtonClicked();
	
	UFUNCTION()
	void HandleLightingButtonClicked();
	
	UFUNCTION()
	void HandleTexturesButtonClicked();
	
	UFUNCTION()
	void HandlePostProcessingButtonClicked();
	
	UFUNCTION()
	void HandleFoliageButtonClicked();

	UFUNCTION()
	void HandleAntiAliasingQualitySelected(int32 SelectedQuality);

	UFUNCTION()
	void HandleViewDistanceQualitySelected(int32 SelectedQuality);

	UFUNCTION()
	void HandleEffectsQualitySelected(int32 SelectedQuality);

	UFUNCTION()
	void HandleLightingQualitySelected(int32 SelectedQuality);

	UFUNCTION()
	void HandleTexturesQualitySelected(int32 SelectedQuality);

	UFUNCTION()
	void HandlePostProcessingQualitySelected(int32 SelectedQuality);

	UFUNCTION()
	void HandleFoliageQualitySelected(int32 SelectedQuality);
	
	UFUNCTION()
	void HandleResolutionScaleSliderValueChanged(float NewValue);

};
