// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollButtonWidget.generated.h"

class UBorder;
class UButton;
class UImage;
class USlider;
class USizeBox;
class UTextBlock;
class UProgressBar;

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
	UButton* PrimaryButton;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* DisplayText;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UImage* CheckBoxImage;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	USlider* OptionsSlider;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UProgressBar* OptionsSliderFillBar;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* SliderText;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* InfoText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	USizeBox* SliderSizeBox;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pseudo Constants")
	bool bHasCheckBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pseudo Constants")
	bool bHasSlider;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pseudo Constants")
	FText TextToDisplay;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pseudo Constants")
	FText InfoTextToDisplay;
	
	// --------------------------------------
	//  Utility
	// --------------------------------------

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	bool bIsChecked;


};
