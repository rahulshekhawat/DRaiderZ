// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatValueWidget.generated.h"

class URichTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UStatValueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UStatValueWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* StatNameTextBlock;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* StatValueTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText StatName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText StatValue;

public:

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetStatName(const FString& InValue);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetStatValue(const FString& InValue, bool bAdd = true);


};
