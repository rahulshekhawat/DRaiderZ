// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuWidgetBase.h"
#include "SettingsWidget.generated.h"


/**
 * 
 */
UCLASS()
class EOD_API USettingsWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	USettingsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;



};
