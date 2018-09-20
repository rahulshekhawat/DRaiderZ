// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

class UEODButton;

/**
 * 
 */
UCLASS()
class EOD_API USkillBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	USkillBarWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// UPROPERTY(meta = (BindWidget))
	// TArray<UEODButton*> Skills;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_1;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_2;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_3;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_4;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_5;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_6;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_7;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_8;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_9;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_10;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_11;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_12;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_13;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_14;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_15;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_16;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_17;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_18;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_19;

	UPROPERTY(meta = (BindWidget))
	UEODButton* Skill_20;

	void PressSkillButton(uint32 SkillIndex);

	void ReleaseSkillButton(uint32 SkillIndex);
	
};
