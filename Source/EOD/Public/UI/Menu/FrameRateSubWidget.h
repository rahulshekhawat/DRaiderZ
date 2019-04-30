// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrameRateSubWidget.generated.h"

class UVideoOptionsWidget;
class UScrollButtonWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFrameRateSelectedMCDelegate, float, SelectedFrameRate);

/**
 * 
 */
UCLASS()
class EOD_API UFrameRateSubWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UFrameRateSubWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* F30;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* F60;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* F90;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget * F120;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget * Unlimited;

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
	FFrameRateSelectedMCDelegate OnFrameRateSelected;

protected:

	UFUNCTION()
	void HandleF30Clicked();

	UFUNCTION()
	void HandleF60Clicked();

	UFUNCTION()
	void HandleF90Clicked();

	UFUNCTION()
	void HandleF120Clicked();

	UFUNCTION()
	void HandleUnlimitedClicked();

};
