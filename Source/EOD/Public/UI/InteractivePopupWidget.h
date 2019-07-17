// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractivePopupWidget.generated.h"

class UImage;
class UTexture;
class URichTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UInteractivePopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UInteractivePopupWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	//~

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	UImage* IconImage;

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* KeyText;

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* DetailText;

	UFUNCTION(BlueprintCallable, Category = "UI Setters")
	void SetDetailText(FText Text);

	UFUNCTION(BlueprintCallable, Category = "UI Setters")
	void SetKeyText(FText Text);

	UFUNCTION(BlueprintCallable, Category = "UI Setters")
	void SetIcon(UTexture* InTexture);

};
