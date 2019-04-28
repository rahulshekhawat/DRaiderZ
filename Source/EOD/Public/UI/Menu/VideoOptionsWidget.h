// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/OptionsWidgetBase.h"
#include "VideoOptionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UVideoOptionsWidget : public UOptionsWidgetBase
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UVideoOptionsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	virtual void CloseDownOptions() override;

	
};
