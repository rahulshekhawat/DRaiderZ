// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeItemContainer.h"
#include "EODPreprocessors.h"

USkillTreeItemContainer::USkillTreeItemContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EEODContainerType::SkillTree;
}

bool USkillTreeItemContainer::Initialize()
{
	if (Super::Initialize() && SkillUpgradeText)
	{
		LoadDefaultSkillTreeContainerState();
		return true;
	}
	return false;

	/*
	LoadSkillContainerState();
	LoadEODItemInfo();
	*/
}

void USkillTreeItemContainer::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeItemContainer::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillTreeItemContainer::RefreshContainerVisuals()
{
	Super::RefreshContainerVisuals();
	UpdateSkillUpgradeText();
}

void USkillTreeItemContainer::UpdateContainerSkillState(FSkillState& FromSkillState)
{
	SkillState = FromSkillState;
	// @todo update container
}

void USkillTreeItemContainer::LoadDefaultSkillTreeContainerState()
{
	if (SkillGroup == FString(""))
	{
		return;
	}

	FString SkillID = FString("F_") + SkillGroup + FString("_1");
	FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(FName(*SkillID), FString("USkillTreeItemContainer::LoadDefaultSkillTreeContainerState(), looking for player skill"));
	if (!Skill)
	{
		return;
	}

	if (IsValid(Skill->Icon))
	{
		EODItemInfo.Icon = Skill->Icon;
	}

	SkillState.bUnlocked = false;
	SkillState.CurrentUpgradeLevel = 0;
	SkillState.MaxUpgradeLevel = Skill->MaxUpgrades;

	RefreshContainerVisuals();
}
