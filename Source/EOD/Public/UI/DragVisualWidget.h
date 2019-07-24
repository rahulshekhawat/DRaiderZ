// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragVisualWidget.generated.h"

class UTexture;

/**
 * 
 */
UCLASS()
class EOD_API UDragVisualWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UDragVisualWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
	UTexture* DragIcon;
	
};
