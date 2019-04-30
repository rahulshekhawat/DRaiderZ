// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary.h"
#include "Blueprint/UserWidget.h"
#include "ScrollButtonWidget.generated.h"

class UBorder;
class UImage;
class USlider;
class UButton;
class USizeBox;
class UTextBlock;
class UProgressBar;
class UMaterialInterface;

/**
 * 
 */
UCLASS()
class EOD_API UScrollButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UScrollButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UBorder* RootBorder;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* MainButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* DisplayTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* InfoTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* SliderValueTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	USizeBox* SliderSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	USlider* OptionsSlider;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UProgressBar* SliderProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UImage* CheckBoxImage;
	
public:

	// --------------------------------------
	//  Widget Behaviour
	// --------------------------------------

	FORCEINLINE FText GetDisplayText() const { return DisplayText; }

	FORCEINLINE bool IsChecked() const { return bIsChecked; }

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void SetDisplayText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void SetInfoText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void SetChecked(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void SetSliderValue(float InValue);

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	FText GetText() const;

	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	bool IsOptionChecked() const;

protected:

	/** Determines whether this button is currently checked */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behaviour")
	bool bIsChecked;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText InfoText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bHasCheckBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bHasSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UMaterialInterface* CheckedMaterialInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UMaterialInterface* UncheckedMaterialInterface;
	
public:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FCustomWidgetEventMCDelegate OnClicked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FSliderChangeMCDelegate OnSliderValueChanged;

protected:

	UFUNCTION()
	virtual void OnMainButtonClicked();

	UFUNCTION()
	void OnOptionSliderValueChanged(float NewValue);

	// --------------------------------------
	//  Handle Mouse Events
	// --------------------------------------

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
