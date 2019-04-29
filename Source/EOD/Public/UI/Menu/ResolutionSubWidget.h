// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResolutionSubWidget.generated.h"

class UScrollBox;
class UVideoOptionsWidget;
class UScrollButtonWidget;

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
	TArray<UScrollButtonWidget*> ResolutionWidgets;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Classes")
	TSubclassOf<UScrollButtonWidget> ScrollButtonWidgetClass;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetParentOptionsWidget(UVideoOptionsWidget* NewParent);
	
protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	TArray<FString> AvailableResolutions;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	UVideoOptionsWidget* ParentOptionsWidget;

	void GenerateAvailableResolutionButtons();

};
