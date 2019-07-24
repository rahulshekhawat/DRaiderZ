// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "EODDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UEODDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UEODDragDropOperation(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DragAndDrop)
	class UContainerWidgetBase* DragOpSourceContainer;
	
};
