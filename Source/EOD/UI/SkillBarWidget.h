// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

class UEODItemContainer;
class APlayerCharacter;

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

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_1;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_2;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_3;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_4;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_5;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_6;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_7;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_8;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_9;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_10;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_11;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_12;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_13;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_14;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_15;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_16;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_17;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_18;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_19;

	UPROPERTY(meta = (BindWidget))
	UEODItemContainer* Skill_20;

	/** Get SkillID of skill placed at SkillIndex */
	FORCEINLINE FName GetSkillAtIndex(int32 SkillIndex) const;

	/** Returns true if skill at given skill index is in cooldown */
	FORCEINLINE bool IsSkillInCooldown(int32 SkillIndex) const;

	/** Put skill at given skill index on cooldown */
	FORCEINLINE void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	/** Save current skill bar layout to the current save slot */
	FORCEINLINE void SaveSkillBarLayout();

private:

	/** Returns EOD item container at given skill index */
	FORCEINLINE UEODItemContainer* GetSkillButtonAtIndex(int32 ButtonIndex) const;

	/** Get the owning EOD player of this widget */
	FORCEINLINE APlayerCharacter* GetOwningEODPlayer() const;

	/** Load previously saved skill bar layout from current save slot */
	FORCEINLINE void LoadSkillBarLayout();

	// void PressSkillButton(uint32 SkillIndex);

	// void ReleaseSkillButton(uint32 SkillIndex);


};
