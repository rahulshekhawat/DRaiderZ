// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/EODItemContainer.h"
#include "Player/PlayerCharacter.h"
#include "Statics/CharacterLibrary.h"

#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

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
	inline FName GetSkillIDAtIndex(const int32 SkillIndex) const;

	inline FString GetSkillGroupAtIndex(const int32 SkillIdex) const;

	inline FEODItemInfo GetEODItemInfoAtIndex(const int32 SkillIndex) const;

	/** Returns true if skill at given skill index is in cooldown */
	inline bool IsSkillInCooldown(int32 SkillIndex) const;

	bool IsSkillGroupInCooldown(const FString& SkillGroup) const;

	/** Put skill at given skill index on cooldown */
	void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	void PutSkillOnCooldownTimer(FString SkillGroup, float Duration, float Interval);

	/** Save current skill bar layout to the current save slot */
	void SaveSkillBarLayout();

private:

	/** Returns EOD item container at given skill index */
	inline UEODItemContainer* GetSkillButtonAtIndex(int32 ButtonIndex) const;

	/** Get the owning EOD player of this widget */
	FORCEINLINE APlayerCharacter* GetOwningEODPlayer() const;

	/** Load previously saved skill bar layout from current save slot */
	void LoadSkillBarLayout();

};

inline FName USkillBarWidget::GetSkillIDAtIndex(const int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillButtonAtIndex(SkillIndex);
	if (Skill)
	{
		return Skill->EODItemInfo.ItemID;
	}

	return NAME_None;
}

inline FString USkillBarWidget::GetSkillGroupAtIndex(const int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillButtonAtIndex(SkillIndex);
	if (Skill)
	{
		return Skill->EODItemInfo.ItemGroup;
	}

	return FString("");
}

inline FEODItemInfo USkillBarWidget::GetEODItemInfoAtIndex(const int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillButtonAtIndex(SkillIndex);
	if (Skill)
	{
		return Skill->EODItemInfo;
	}

	return FEODItemInfo();
}

inline bool USkillBarWidget::IsSkillInCooldown(int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillButtonAtIndex(SkillIndex);
	if (Skill)
	{
		return Skill->bInCooldown;
	}

	return false;
}

inline UEODItemContainer* USkillBarWidget::GetSkillButtonAtIndex(int32 ButtonIndex) const
{
	switch (ButtonIndex)
	{
	case 1:
		return Skill_1;
	case 2:
		return Skill_2;
	case 3:
		return Skill_3;
	case 4:
		return Skill_4;
	case 5:
		return Skill_5;
	case 6:
		return Skill_6;
	case 7:
		return Skill_7;
	case 8:
		return Skill_8;
	case 9:
		return Skill_9;
	case 10:
		return Skill_10;
	case 11:
		return Skill_11;
	case 12:
		return Skill_12;
	case 13:
		return Skill_13;
	case 14:
		return Skill_14;
	case 15:
		return Skill_15;
	case 16:
		return Skill_16;
	case 17:
		return Skill_17;
	case 18:
		return Skill_18;
	case 19:
		return Skill_19;
	case 20:
		return Skill_20;
	default:
		break;
	}

	return nullptr;
}

FORCEINLINE APlayerCharacter* USkillBarWidget::GetOwningEODPlayer() const
{
	APlayerCharacter* OwningEODPlayer = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	return OwningEODPlayer;
}
