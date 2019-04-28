// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidgetBase.generated.h"

class USettingsWidget;

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
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void CloseDownOptions();

	UFUNCTION(BlueprintCallable, Category = "Utility")
	virtual void ResetOptions();

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	bool bIsDirty;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	USettingsWidget* SettingsParentWidget;


};
