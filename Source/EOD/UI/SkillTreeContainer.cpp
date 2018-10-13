// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeContainer.h"

#include "Engine/Engine.h"

USkillTreeContainer::USkillTreeContainer(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EEODContainerType::SkillTree;
}

bool USkillTreeContainer::Initialize()
{
	if (Super::Initialize() &&
		SkillUpgradeText)
	{
		return true;
	}

	return false;
}

void USkillTreeContainer::NativeConstruct()
{
	Super::NativeConstruct();

}

void USkillTreeContainer::NativeDestruct()
{
	Super::NativeDestruct();

}
