// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/OptionsWidgetBase.h"
#include "AudioOptionsWidget.generated.h"

class UGameUserSettings;
class UScrollButtonWidget;
class UQualitySettingSubWidget;

/**
 * 
 */
UCLASS()
class EOD_API UAudioOptionsWidget : public UOptionsWidgetBase
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UAudioOptionsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* AudioQuality;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Master;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Music;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Voice;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* SFX;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UQualitySettingSubWidget* AudioQualitySub;

	// --------------------------------------
	//  Widget Update
	// --------------------------------------

	virtual void InitializeOptions() override;
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void UpdateCurrentAudioQuality(UGameUserSettings* GameUserSettings = nullptr);
	
protected:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UFUNCTION()
	void HandleAudioQualityButtonClicked();

};
