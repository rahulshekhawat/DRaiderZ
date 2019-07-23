// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ContainerWidgetBase.h"
#include "SkillTreeContainerWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeContainerWidget : public UContainerWidgetBase
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillTreeContainerWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void SetDataObj(UObject* InDataObj) override;

	FContainerMCDelegate OnUpgradeButtonClicked;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* UpgradeButton;

	UFUNCTION()
	virtual void UpgradeButtonClicked();
	
};
