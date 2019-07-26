// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ContainerWidgetBase.h"
#include "InventoryContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UInventoryContainerWidget : public UContainerWidgetBase
{
	GENERATED_BODY()
		
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UInventoryContainerWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void SetDataObj(UObject* InDataObj) override;

	void SetItemCount(int32 InValue);

	
};
