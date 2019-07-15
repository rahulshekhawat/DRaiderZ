// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeEnvelopWidget.generated.h"

class UDynamicSkillTreeWidget;

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeEnvelopWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	USkillTreeEnvelopWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UDynamicSkillTreeWidget* SkillTreeWidget;

	
};
