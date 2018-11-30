// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UDialogueWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UDialogueWindowWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

};
