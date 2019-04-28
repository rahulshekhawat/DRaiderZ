// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RegularButtonWidget.generated.h"

class UBorder;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API URegularButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	URegularButtonWidget(const FObjectInitializer& ObjectInitializer);

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

	UFUNCTION()
	void OnPrimaryButtonClicked();

	UFUNCTION()
	void OnPrimaryButtonHovered();

	UFUNCTION()
	void OnPrimaryButtonUnhovered();

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pseudo Constants")
	FText TextToDisplay;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	bool bIsLocked;

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void LockButton(bool bInLocked);

};
