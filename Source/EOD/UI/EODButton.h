// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "EODButton.generated.h"

/**
 * EODButton expands upon UE4's UButton to provide functionality and support for inventory buttons, skill bar buttons, etc.
 */
UCLASS()
class EOD_API UEODButton : public UButton
{
	GENERATED_BODY()
	
public:

	UEODButton(const FObjectInitializer& ObjectInitializer);

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODButtonInfo)
	// EEODItemType EODItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODButtonInfo)
	bool bCanBeClicked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODButtonInfo)
	bool bCanBeDragged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODButtonInfo)
	FName EODItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODButtonInfo)
	UTexture* EODItemIcon;

	// void ButtonClicked();

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = InGameData)
		



};
