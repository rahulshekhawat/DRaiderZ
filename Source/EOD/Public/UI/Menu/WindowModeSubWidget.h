// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WindowModeSubWidget.generated.h"

class UScrollButtonWidget;
class UVideoOptionsWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWindowModeSelectedMCDelegate, EWindowMode::Type, SelectedWindowMode);

/**
 * 
 */
UCLASS()
class EOD_API UWindowModeSubWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UWindowModeSubWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Window;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Fullscreen;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Borderless;
	
	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetParentOptionsWidget(UVideoOptionsWidget* NewParent);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	UVideoOptionsWidget* ParentOptionsWidget;
	
public:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FWindowModeSelectedMCDelegate OnWindowModeSelected;

protected:

	UFUNCTION()
	void HandleWindowButtonClicked();

	UFUNCTION()
	void HandleFullscreenButtonClicked();

	UFUNCTION()
	void HandleBorderlessButtonClicked();

};
