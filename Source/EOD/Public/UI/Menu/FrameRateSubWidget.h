// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrameRateSubWidget.generated.h"

class UVideoOptionsWidget;

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
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetParentOptionsWidget(UVideoOptionsWidget* NewParent);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	UVideoOptionsWidget* ParentOptionsWidget;


};
