// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResolutionSubWidget.generated.h"

class UScrollBox;
class UVideoOptionsWidget;
class UResolutionButtonWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResolutionSelectedMCDelegate, FIntPoint, SelectedResolution);

/**
 * 
 */
UCLASS()
class EOD_API UResolutionSubWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UResolutionSubWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	/** Scroll box containing supported screen resolutions */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollBox* ResolutionScroller;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Container Child")
	TArray<UResolutionButtonWidget*> ResolutionWidgets;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Classes")
	TSubclassOf<UResolutionButtonWidget> ScrollButtonWidgetClass;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetParentOptionsWidget(UVideoOptionsWidget* NewParent);
	
protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	TArray<FIntPoint> AvailableResolutions;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	UVideoOptionsWidget* ParentOptionsWidget;

	void GenerateAvailableResolutionButtons();

public:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FResolutionSelectedMCDelegate OnResolutionSelected;

protected:

	UFUNCTION()
	void HandleResolutionButtonClicked(FIntPoint ButtonResolution);

};
