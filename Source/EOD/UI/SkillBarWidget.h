// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/UI/EODItemContainer.h"
#include "EOD/Characters/PlayerCharacter.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"


/** Delegate for when a new skill from skill tree is dropped on skill bar */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewSkillAddedMCDelegate, int32, SkillBarIndex, FString, SkillGroup);

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

public:
	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_1;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_2;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_3;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_4;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_5;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_6;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_7;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_8;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_9;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_10;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_11;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_12;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_13;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_14;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_15;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_16;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_17;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_18;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_19;

	UPROPERTY(BlueprintReadOnly, Category = "Skill Bar", meta = (BindWidget))
	UEODItemContainer* SkillContainer_20;

public:
	UPROPERTY(Transient)
	TMap<int32, UEODItemContainer*> IndexToContainerMap;

	UPROPERTY(BlueprintAssignable, Category = "Skill Bar")
	FOnNewSkillAddedMCDelegate OnNewSkillAdded;

public:
	/** Get ID of skill placed at given index of skill bar */
	inline FName GetSkillIDAtIndex(const int32 ContainerIndex) const;

	/** Get SkillGroup of skill placed at given index of skill bar */
	inline FString GetSkillGroupAtIndex(const int32 ContainerIndex) const;

	/** Get EODItemInfo of EODItemContainer placed at given index of skill bar */
	inline FEODItemInfo GetEODItemInfoAtIndex(const int32 ContainerIndex) const;

	/** Returns true if skill at given skill index is in cooldown */
	inline bool IsSkillInCooldown(const int32 SkillIndex) const;

	/** Returns true the given skill container exists on skill bar */
	inline bool HasSkillContainer(UEODItemContainer* Container) const;

	/** Returns true if the given skill group is in cooldown */
	bool IsSkillGroupInCooldown(const FString& SkillGroup) const;

	/** Put skill at given skill index on cooldown */
	void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	void PutSkillOnCooldownTimer(FString SkillGroup, float Duration, float Interval);

	/** Save current skill bar layout to the current save slot */
	void SaveSkillBarLayout();

	/** Called when a new skill is dropped on skill bar from skill tree */
	bool OnNewSkillDropped(UEODItemContainer* FromContainer, UEODItemContainer* ToContainer);

	bool OnSkillRemoved(UEODItemContainer* Container);
	
	bool OnSkillsSwapped(UEODItemContainer* Container1, UEODItemContainer* Container2);

	void UpdateSkillBarLayout(TMap<int32, FString>& NewLayout, bool bResize = false);

private:
	inline void InitializeSkillContainer(UEODItemContainer* Container, const int32 ContainerIndex);

	/** Returns EOD item container at given skill index */
	inline UEODItemContainer* GetSkillContainerAtIndex(const int32 Index) const;

	/** Returns the skill index of given container. Returns -1 if the container couldn't be found. */
	inline int32 GetIndexOfSkillContainer(UEODItemContainer* Container) const;

	/** Get the owning EOD player of this widget */
	FORCEINLINE APlayerCharacter* GetOwningEODPlayer() const;

};

inline FName USkillBarWidget::GetSkillIDAtIndex(const int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillContainerAtIndex(SkillIndex);
	return IsValid(Skill) ? Skill->EODItemInfo.ItemID : NAME_None;
}

inline FString USkillBarWidget::GetSkillGroupAtIndex(const int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillContainerAtIndex(SkillIndex);
	if (IsValid(Skill))
	{
		return Skill->EODItemInfo.ItemGroup;
	}
	return FString("");
}

inline FEODItemInfo USkillBarWidget::GetEODItemInfoAtIndex(const int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillContainerAtIndex(SkillIndex);
	if (Skill)
	{
		return Skill->EODItemInfo;
	}

	return FEODItemInfo();
}

inline bool USkillBarWidget::IsSkillInCooldown(int32 SkillIndex) const
{
	UEODItemContainer* Skill = GetSkillContainerAtIndex(SkillIndex);
	if (Skill)
	{
		return Skill->bInCooldown;
	}

	return false;
}

inline bool USkillBarWidget::HasSkillContainer(UEODItemContainer* Container) const
{
	if (IsValid(Container))
	{
		for (int i = 1; i <= 20; i++)
		{
			if (Container == GetSkillContainerAtIndex(i))
			{
				return true;
			}
		}
	}

	return false;
}

inline void USkillBarWidget::InitializeSkillContainer(UEODItemContainer* Container, const int32 SkillBarIndex)
{
	if (IsValid(Container))
	{
		Container->SetParentWidget(this);
		if (!IndexToContainerMap.Contains(SkillBarIndex))
		{
			IndexToContainerMap.Add(SkillBarIndex, Container);
		}
	}
}

inline UEODItemContainer* USkillBarWidget::GetSkillContainerAtIndex(const int32 ContainerIndex) const
{
	switch (ContainerIndex)
	{
	case 1:
		return SkillContainer_1;
	case 2:
		return SkillContainer_2;
	case 3:
		return SkillContainer_3;
	case 4:
		return SkillContainer_4;
	case 5:
		return SkillContainer_5;
	case 6:
		return SkillContainer_6;
	case 7:
		return SkillContainer_7;
	case 8:
		return SkillContainer_8;
	case 9:
		return SkillContainer_9;
	case 10:
		return SkillContainer_10;
	case 11:
		return SkillContainer_11;
	case 12:
		return SkillContainer_12;
	case 13:
		return SkillContainer_13;
	case 14:
		return SkillContainer_14;
	case 15:
		return SkillContainer_15;
	case 16:
		return SkillContainer_16;
	case 17:
		return SkillContainer_17;
	case 18:
		return SkillContainer_18;
	case 19:
		return SkillContainer_19;
	case 20:
		return SkillContainer_20;
	default:
		break;
	}

	return nullptr;
}

inline int32 USkillBarWidget::GetIndexOfSkillContainer(UEODItemContainer* Container) const
{
	for (int i = 1; i <= 20; i++)
	{
		if (Container == GetSkillContainerAtIndex(i))
		{
			return i;
		}
	}

	// return invalid index if the container couldn't be found
	return -1;
}

FORCEINLINE APlayerCharacter* USkillBarWidget::GetOwningEODPlayer() const
{
	return Cast<APlayerCharacter>(GetOwningPlayerPawn());
}
