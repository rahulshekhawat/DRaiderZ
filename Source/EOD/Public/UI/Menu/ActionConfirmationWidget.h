// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionConfirmationWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConfirmationButtonClickMCDelegate);

/**
 * 
 */
UCLASS()
class EOD_API UActionConfirmationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UActionConfirmationWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* ConfirmationTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* YesButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* NoButton;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Psuedo Constants")
	FText ConfirmationText;

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UI Events")
	FConfirmationButtonClickMCDelegate OnYesButtonClicked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "UI Events")
	FConfirmationButtonClickMCDelegate OnNoButtonClicked;

protected:

	UFUNCTION()
	void HandleYesButtonClicked();

	UFUNCTION()
	void HandleNoButtonClicked();

};
