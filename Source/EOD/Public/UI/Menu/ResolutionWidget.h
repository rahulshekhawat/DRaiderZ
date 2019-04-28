// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResolutionWidget.generated.h"

class UScrollBox;

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

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	/** Scroll box containing supported screen resolutions */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollBox* ResolutionScroller;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	TArray<FString> AvailableResolutions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<UUserWidget> ResolutionButtonClass;

	void GenerateAvailableResolutionButtons();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void AddResolutionButtonToScrollBox(const FString& Resolution);
	
};
