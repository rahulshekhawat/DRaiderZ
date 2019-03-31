// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ContainerDragDropOperation.generated.h"

class UContainerWidget;

/**
 * 
 */
UCLASS()
class EOD_API UContainerDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UContainerDragDropOperation(const FObjectInitializer& ObjectInitializer);

	/** Reference to container widget that is currently being dragged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Payload)
	UContainerWidget* DraggedContainerWidget;


	
};
