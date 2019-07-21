// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

class USkillBarContainerWidget;

/**
 * 
 */
UCLASS()
class EOD_API USkillBarWidget : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_11;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_13;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_14;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_17;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_18;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_19;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_20;



};
