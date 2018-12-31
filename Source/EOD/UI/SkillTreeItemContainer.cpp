// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeItemContainer.h"

#include "Kismet/KismetSystemLibrary.h"

USkillTreeItemContainer::USkillTreeItemContainer(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EEODContainerType::SkillTree;
}

bool USkillTreeItemContainer::Initialize()
{
	if (!(Super::Initialize() && SkillUpgradeText))
	{
		return false;
	}

	LoadSkillContainerState();
	LoadEODItemInfo();
	RefreshContainerVisuals();

	return true;
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
