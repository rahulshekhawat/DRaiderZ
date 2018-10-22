// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UI/EODItemContainer.h"
#include "Player/PlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

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
	inline FName GetSkillAtIndex(int32 SkillIndex) const;

	/** Returns true if skill at given skill index is in cooldown */
	inline bool IsSkillInCooldown(int32 SkillIndex) const;

	/** Put skill at given skill index on cooldown */
	inline void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	/** Save current skill bar layout to the current save slot */
	void SaveSkillBarLayout();

private:

	/** Returns EOD item container at given skill index */
	inline UEODItemContainer* GetSkillButtonAtIndex(int32 ButtonIndex) const;

	/** Get the owning EOD player of this widget */
	FORCEINLINE APlayerCharacter* GetOwningEODPlayer() const;

	/** Load previously saved skill bar layout from current save slot */
	void LoadSkillBarLayout();

	// void PressSkillButton(uint32 SkillIndex);

	// void ReleaseSkillButton(uint32 SkillIndex);


};

inline FName USkillBarWidget::GetSkillAtIndex(int32 SkillIndex) const
{
	switch (SkillIndex)
	{
	case 1:
		return Skill_1->EODItemInfo.ItemID;
	case 2:
		return Skill_2->EODItemInfo.ItemID;
	case 3:
		return Skill_3->EODItemInfo.ItemID;
	case 4:
		return Skill_4->EODItemInfo.ItemID;
	case 5:
		return Skill_5->EODItemInfo.ItemID;
	case 6:
		return Skill_6->EODItemInfo.ItemID;
	case 7:
		return Skill_7->EODItemInfo.ItemID;
	case 8:
		return Skill_8->EODItemInfo.ItemID;
	case 9:
		return Skill_9->EODItemInfo.ItemID;
	case 10:
		return Skill_10->EODItemInfo.ItemID;
	case 11:
		return Skill_11->EODItemInfo.ItemID;
	case 12:
		return Skill_12->EODItemInfo.ItemID;
	case 13:
		return Skill_13->EODItemInfo.ItemID;
	case 14:
		return Skill_14->EODItemInfo.ItemID;
	case 15:
		return Skill_15->EODItemInfo.ItemID;
	case 16:
		return Skill_16->EODItemInfo.ItemID;
	case 17:
		return Skill_17->EODItemInfo.ItemID;
	case 18:
		return Skill_18->EODItemInfo.ItemID;
	case 19:
		return Skill_19->EODItemInfo.ItemID;
	case 20:
		return Skill_20->EODItemInfo.ItemID;
	default:
		break;
	}

	return NAME_None;
}

inline bool USkillBarWidget::IsSkillInCooldown(int32 SkillIndex) const
{
	switch (SkillIndex)
	{
	case 1:
		return Skill_1->bInCooldown;
	case 2:
		return Skill_2->bInCooldown;
	case 3:
		return Skill_3->bInCooldown;
	case 4:
		return Skill_4->bInCooldown;
	case 5:
		return Skill_5->bInCooldown;
	case 6:
		return Skill_6->bInCooldown;
	case 7:
		return Skill_7->bInCooldown;
	case 8:
		return Skill_8->bInCooldown;
	case 9:
		return Skill_9->bInCooldown;
	case 10:
		return Skill_10->bInCooldown;
	case 11:
		return Skill_11->bInCooldown;
	case 12:
		return Skill_12->bInCooldown;
	case 13:
		return Skill_13->bInCooldown;
	case 14:
		return Skill_14->bInCooldown;
	case 15:
		return Skill_15->bInCooldown;
	case 16:
		return Skill_16->bInCooldown;
	case 17:
		return Skill_17->bInCooldown;
	case 18:
		return Skill_18->bInCooldown;
	case 19:
		return Skill_19->bInCooldown;
	case 20:
		return Skill_20->bInCooldown;
	default:
		break;
	}

	return false;
}

inline void USkillBarWidget::PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval)
{
	switch (SkillIndex)
	{
	case 1:
		Skill_1->StartCooldown(Duration, Interval);
		break;
	case 2:
		Skill_2->StartCooldown(Duration, Interval);
		break;
	case 3:
		Skill_3->StartCooldown(Duration, Interval);
		break;
	case 4:
		Skill_4->StartCooldown(Duration, Interval);
		break;
	case 5:
		Skill_5->StartCooldown(Duration, Interval);
		break;
	case 6:
		Skill_6->StartCooldown(Duration, Interval);
		break;
	case 7:
		Skill_7->StartCooldown(Duration, Interval);
		break;
	case 8:
		Skill_8->StartCooldown(Duration, Interval);
		break;
	case 9:
		Skill_9->StartCooldown(Duration, Interval);
		break;
	case 10:
		Skill_10->StartCooldown(Duration, Interval);
		break;
	case 11:
		Skill_11->StartCooldown(Duration, Interval);
		break;
	case 12:
		Skill_12->StartCooldown(Duration, Interval);
		break;
	case 13:
		Skill_13->StartCooldown(Duration, Interval);
		break;
	case 14:
		Skill_14->StartCooldown(Duration, Interval);
		break;
	case 15:
		Skill_15->StartCooldown(Duration, Interval);
		break;
	case 16:
		Skill_16->StartCooldown(Duration, Interval);
		break;
	case 17:
		Skill_17->StartCooldown(Duration, Interval);
		break;
	case 18:
		Skill_18->StartCooldown(Duration, Interval);
		break;
	case 19:
		Skill_19->StartCooldown(Duration, Interval);
		break;
	case 20:
		Skill_20->StartCooldown(Duration, Interval);
		break;
	default:
		break;
	}
}

inline UEODItemContainer * USkillBarWidget::GetSkillButtonAtIndex(int32 ButtonIndex) const
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
