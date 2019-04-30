// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidgetBase.generated.h"

class UScrollBox;
class UWidgetSwitcher;
class USettingsWidget;
class UScrollButtonWidget;

/**
 * 
 */
UCLASS()
class EOD_API UOptionsWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UOptionsWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollBox* OptionsScrollBox;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UWidgetSwitcher* SubWidgetSwitcher;

	// --------------------------------------
	//  Widget Update
	// --------------------------------------

	virtual void InitializeOptions(bool bForceUserSettingReload = false);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void CloseDownOptions();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void ResetOptions();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void EnableScrollBoxItems();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void DisableScrollBoxItems(UWidget* ExcludedItem);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void ToggleSubOptions(UWidget* SubWidget, UScrollButtonWidget* CallingScrollButton);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void GetQualityText(int32 InQualityLevel, FText& OutText);

	// --------------------------------------
	//  Utility
	// --------------------------------------

	FORCEINLINE USettingsWidget* GetSettingsParentWidget() const { return SettingsParentWidget; }

	FORCEINLINE bool IsDirty() const { return bIsDirty; }

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetSettingsParentWidget(USettingsWidget* NewParent);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetDirty(bool bValue);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	bool bIsDirty;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	USettingsWidget* SettingsParentWidget;


};
