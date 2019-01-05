// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Core/EODSaveGame.h"
#include "EOD/Core/GameSingleton.h"
#include "EOD/UI/EODItemContainer.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Engine/Engine.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SkillTreeItemContainer.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeItemContainer : public UEODItemContainer
{
	GENERATED_BODY()
	
public:
	USkillTreeItemContainer(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	/** Text block displaying skill upgrade in the form : CurrentUpgradeLevel / MaxUpgradeLevel */
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Item Container", meta = (BindWidget))
	UTextBlock* SkillUpgradeText;

	/**
	 * Determines the skill group this skill belongs to
	 * For a skill with SkillID 'BZ1_1', it's skill group is 'BZ1'
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Item Container")
	FString SkillGroup;

	/** Skill group that must be unlocked before unlocking this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Item Container")
	FString SkillGroupRequiredToUnlock;

	/** The skill tree column that this skill container belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Item Container")
	int32 ColumnPosition;

	/** The skill tree row that this skill container belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree Item Container")
	int32 RowPosition;

	/** Current state of this skill */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Skill Tree Item Container")
	FSkillState SkillState;

public:
	/** Refresh and update the displayed visuals of this container */
	virtual void RefreshContainerVisuals() override;

private:
	void LoadDefaultSkillTreeContainerState();

	inline void UpdateSkillUpgradeText();

	/** Load previously saved skill state from current save slot */
	// inline void LoadSkillContainerState();

	// inline void LoadEODItemInfo();

};

inline void USkillTreeItemContainer::UpdateSkillUpgradeText()
{
	FString String;
	String = FString::FromInt(SkillState.CurrentUpgradeLevel) + FString("/") + FString::FromInt(SkillState.MaxUpgradeLevel);
	FText Text = FText::FromString(String);
	if (IsValid(SkillUpgradeText))
	{
		SkillUpgradeText->SetText(Text);
	}
}

/*
inline void USkillTreeItemContainer::LoadSkillContainerState()
{
	if (SkillGroup == FString())
	{
		return;
	}

	UGameSingleton* GameSingleton = IsValid(GEngine) ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	if (!IsValid(GameSingleton))
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex));
	if (!IsValid(EODSaveGame))
	{
		return;
	}

	if (EODSaveGame->SkillToStateMap.Contains(SkillGroup))
	{
		SkillState = EODSaveGame->SkillToStateMap[SkillGroup];
	}
}

inline void USkillTreeItemContainer::LoadEODItemInfo()
{
	if (SkillGroup == FString())
	{
		return;
	}

	FString SkillID;
	if (SkillState.CurrentUpgradeLevel > 0)
	{
		SkillID = FString("F_") + SkillGroup + FString("_") + FString::FromInt(SkillState.CurrentUpgradeLevel);
	}
	else
	{
		SkillID = FString("F_") + SkillGroup + FString("_1");
	}

	FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(FName(*SkillID), FString("USkillTreeItemContainer::LoadEODItemInfo(), looking for player skill"));
	if (!Skill)
	{
		return;
	}

	EODItemInfo.ItemID = FName(*SkillID);
	if (Skill->bPassiveSkill)
	{
		EODItemInfo.EODItemType = EEODItemType::PassiveSkill;
	}
	else
	{
		EODItemInfo.EODItemType = EEODItemType::ActiveSkill;
	}
	EODItemInfo.Icon = Skill->Icon;
	EODItemInfo.ItemGroup = SkillGroup;
	EODItemInfo.Description = Skill->Description;
	EODItemInfo.InGameName = Skill->InGameName;
	EODItemInfo.StackCount = 1;

	SkillState.MaxUpgradeLevel = Skill->MaxUpgrades;
}
*/