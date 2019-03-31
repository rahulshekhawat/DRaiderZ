// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"

class UImage;
class USlider;
class UButton;
class USizeBox;
class UTextBlock;
class UProgressBar;
class UBorder;
class UMaterialInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMenuButtonClickMCDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSliderChangeMCDelegate, float, Value);

/**
 * 
 */
UCLASS()
class EOD_API UMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UMenuButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Child Widgets
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

protected:

	// --------------------------------------
	//	Constants / Defaults
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
	//	Widget Behaviour
	// --------------------------------------

	FORCEINLINE FText GetDisplayText() const { return DisplayText; }

	FORCEINLINE bool IsChecked() const { return bIsChecked; }

	void SetInfoText(FText NewText);

	void SetChecked(bool bValue);

	void SetSliderValue(float InValue);

protected:

	/** Determines whether this button is currently checked */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behaviour")
	bool bIsChecked;

public:

	// --------------------------------------
	//	Delegates
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FMenuButtonClickMCDelegate OnClicked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FSliderChangeMCDelegate OnSliderValueChanged;

private:

	UFUNCTION()
	void OnMainButtonClicked();

	UFUNCTION()
	void OnOptionSliderValueChanged(float NewValue);

protected:

	// --------------------------------------
	//	Handle Mouse Events
	// --------------------------------------

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
