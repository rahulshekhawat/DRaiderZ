// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/ScrollButtonWidget.h"
#include "ResolutionButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResolutionButtonClickedMCDelegate, FIntPoint, ButtonResolution);

/**
 * 
 */
UCLASS()
class EOD_API UResolutionButtonWidget : public UScrollButtonWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UResolutionButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FIntPoint ButtonResolution;

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FResolutionButtonClickedMCDelegate OnClicked_ResolutionPayload;

	virtual void OnMainButtonClicked() override;

	
};
