// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeItemContainer.h"
#include "EOD/Core/EODPreprocessors.h"

USkillTreeItemContainer::USkillTreeItemContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EEODContainerType::SkillTree;
}

bool USkillTreeItemContainer::Initialize()
{
	if (Super::Initialize() && SkillUpgradeText)
	{
		LoadSkillIcon();
		// DisableContainer();
		return true;
	}
	return false;

	/*
	LoadSkillContainerState();
	LoadEODItemInfo();
	RefreshContainerVisuals();
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

void USkillTreeItemContainer::LoadSkillIcon()
{
	if (SkillGroup == FString())
	{
		return;
	}

	FString SkillID = FString("F_") + SkillGroup + FString("_1");
	FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(FName(*SkillID), FString("USkillTreeItemContainer::LoadEODItemInfo(), looking for player skill"));
	if (!Skill || !IsValid(Skill->Icon))
	{
		return;
	}

	EODItemInfo.Icon = Skill->Icon;
	RefreshContainerVisuals();
}
