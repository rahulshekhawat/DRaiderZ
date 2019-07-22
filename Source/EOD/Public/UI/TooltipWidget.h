// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

class UImage;
class UVerticalBox;
class URichTextBlock;
class UStatValueWidget;

/**
 * UTooltipWidget pops up on hovering over a relevant UI element (e.g. inventory item)
 * and is used to display information of that particular item.
 */
UCLASS()
class EOD_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UTooltipWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	UImage* Icon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* TitleText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* SubTitleText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* DescriptionText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	UVerticalBox* StatsVerticalBox;

public:

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetIcon(UTexture* InTexture);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetTitle(const FString& InValue, ERarity Rarity = ERarity::Common);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetSubTitle(const FString& InValue);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetDescription(const FString& InValue);

};
