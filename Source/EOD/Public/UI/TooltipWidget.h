// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

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

	
	
	
};
