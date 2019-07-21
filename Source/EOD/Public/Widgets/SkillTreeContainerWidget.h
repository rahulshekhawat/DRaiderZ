// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ContainerWidgetBase.h"
#include "SkillTreeContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeContainerWidget : public UContainerWidgetBase
{
	GENERATED_BODY()

public:

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides

	USkillTreeContainerWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	class UButton* UpgradeButton;



	
};
