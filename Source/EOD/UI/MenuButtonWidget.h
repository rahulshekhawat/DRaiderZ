// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMenuButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;



};
