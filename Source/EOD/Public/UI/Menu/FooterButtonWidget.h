// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UILibrary.h"
#include "Blueprint/UserWidget.h"
#include "FooterButtonWidget.generated.h"

class UBorder;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UFooterButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UFooterButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Child Widgets
	// --------------------------------------
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* FooterButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UBorder* RootBorder;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* DisplayTextBlock;

protected:

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText DisplayText;

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FCustomWidgetEventMCDelegate OnClicked;

	UFUNCTION()
	void HandleFootButtonClicked();

	UFUNCTION()
	void HandleFootButtonHovered();

	UFUNCTION()
	void HandleFootButtonUnhovered();


};
