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
		return true;
	}

	return false;

	/*
	if (!(Super::Initialize() && SkillUpgradeText))
	{
		return false;
	}

	LoadSkillContainerState();
	LoadEODItemInfo();
	RefreshContainerVisuals();

	return true;
	*/
}

void USkillTreeItemContainer::NativeConstruct()
{
	// Super::NativeConstruct();
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
	if (Skill && IsValid(ItemImage))
	{
		if (IsValid(Skill->Icon))
		{
			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(52.0, 52.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::Image;
			SlateBrush.ImageType = ESlateBrushImageType::FullColor;
			SlateBrush.SetResourceObject(Skill->Icon);
			ItemImage->SetBrush(SlateBrush);
		}
		else
		{
			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(52.0, 52.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
			SlateBrush.ImageType = ESlateBrushImageType::NoImage;
			ItemImage->SetBrush(SlateBrush);
		}
	}
}
