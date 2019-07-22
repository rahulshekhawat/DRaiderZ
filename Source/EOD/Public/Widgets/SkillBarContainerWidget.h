// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ContainerWidgetBase.h"
#include "SkillBarContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API USkillBarContainerWidget : public UContainerWidgetBase
{
	GENERATED_BODY()
		
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillBarContainerWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void SetDataObj(UObject* InDataObj) override;
	
};
