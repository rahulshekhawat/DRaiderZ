// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/EODItemContainer.h"
#include "SkillTreeContainer.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeContainer : public UEODItemContainer
{
	GENERATED_BODY()
	
public:

	USkillTreeContainer(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	FString SkillGroup;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkillUpgradeText;	
	
	// @todo add unlocked status, current unlock level


};
