// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResolutionWidget.generated.h"

class UScrollBox;
class UScrollButtonWidget;

/**
 * 
 */
UCLASS()
class EOD_API UResolutionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UResolutionWidget(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	TArray<FString> AvailableResolutions;

	void GenerateAvailableResolutionButtons();
	
};
