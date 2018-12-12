// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UImage;
class UScrollBox;
class UGridPanel;
class UEODItemContainer;

/**
 * InventoryWidget is the visual representation of player inventory
 */
UCLASS()
class EOD_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UGridPanel* GridPanel;
	*/
	

	
};
