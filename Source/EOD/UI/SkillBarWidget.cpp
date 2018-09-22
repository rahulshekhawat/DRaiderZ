// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBarWidget.h"
#include "UI/EODItemContainer.h"
#include "Player/PlayerCharacter.h"

USkillBarWidget::USkillBarWidget(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
}

bool USkillBarWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void USkillBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillBarWidget::PressSkillButton(uint32 SkillIndex)
{
	if (!OwningEODPlayer)
	{
		// @todo raise error
		return;
	}

	switch (SkillIndex)
	{
	case 1:
		OwningEODPlayer->StartSkill(Skill_1->EODItemID);
		break;
	case 2:
		OwningEODPlayer->StartSkill(Skill_2->EODItemID);
		break;
	case 3:
		OwningEODPlayer->StartSkill(Skill_3->EODItemID);
		break;
	case 4:
		OwningEODPlayer->StartSkill(Skill_4->EODItemID);
		break;
	case 5:
		OwningEODPlayer->StartSkill(Skill_5->EODItemID);
		break;
	case 6:
		OwningEODPlayer->StartSkill(Skill_6->EODItemID);
		break;
	case 7:
		OwningEODPlayer->StartSkill(Skill_7->EODItemID);
		break;
	case 8:
		OwningEODPlayer->StartSkill(Skill_8->EODItemID);
		break;
	case 9:
		OwningEODPlayer->StartSkill(Skill_9->EODItemID);
		break;
	case 10:
		OwningEODPlayer->StartSkill(Skill_10->EODItemID);
		break;
	case 11:
		OwningEODPlayer->StartSkill(Skill_11->EODItemID);
		break;
	case 12:
		OwningEODPlayer->StartSkill(Skill_12->EODItemID);
		break;
	case 13:
		OwningEODPlayer->StartSkill(Skill_13->EODItemID);
		break;
	case 14:
		OwningEODPlayer->StartSkill(Skill_14->EODItemID);
		break;
	case 15:
		OwningEODPlayer->StartSkill(Skill_15->EODItemID);
		break;
	case 16:
		OwningEODPlayer->StartSkill(Skill_16->EODItemID);
		break;
	case 17:
		OwningEODPlayer->StartSkill(Skill_17->EODItemID);
		break;
	case 18:
		OwningEODPlayer->StartSkill(Skill_18->EODItemID);
		break;
	case 19:
		OwningEODPlayer->StartSkill(Skill_19->EODItemID);
		break;
	case 20:
		OwningEODPlayer->StartSkill(Skill_20->EODItemID);
		break;
	default:
		break;
	}
}

void USkillBarWidget::ReleaseSkillButton(uint32 SkillIndex)
{
	if (!OwningEODPlayer)
	{
		// @todo raise error
		return;
	}

	switch (SkillIndex)
	{
	case 1:
		OwningEODPlayer->StopSkill(Skill_1->EODItemID);
		break;
	case 2:
		OwningEODPlayer->StopSkill(Skill_2->EODItemID);
		break;
	case 3:
		OwningEODPlayer->StopSkill(Skill_3->EODItemID);
		break;
	case 4:
		OwningEODPlayer->StopSkill(Skill_4->EODItemID);
		break;
	case 5:
		OwningEODPlayer->StopSkill(Skill_5->EODItemID);
		break;
	case 6:
		OwningEODPlayer->StopSkill(Skill_6->EODItemID);
		break;
	case 7:
		OwningEODPlayer->StopSkill(Skill_7->EODItemID);
		break;
	case 8:
		OwningEODPlayer->StopSkill(Skill_8->EODItemID);
		break;
	case 9:
		OwningEODPlayer->StopSkill(Skill_9->EODItemID);
		break;
	case 10:
		OwningEODPlayer->StopSkill(Skill_10->EODItemID);
		break;
	case 11:
		OwningEODPlayer->StopSkill(Skill_11->EODItemID);
		break;
	case 12:
		OwningEODPlayer->StopSkill(Skill_12->EODItemID);
		break;
	case 13:
		OwningEODPlayer->StopSkill(Skill_13->EODItemID);
		break;
	case 14:
		OwningEODPlayer->StopSkill(Skill_14->EODItemID);
		break;
	case 15:
		OwningEODPlayer->StopSkill(Skill_15->EODItemID);
		break;
	case 16:
		OwningEODPlayer->StopSkill(Skill_16->EODItemID);
		break;
	case 17:
		OwningEODPlayer->StopSkill(Skill_17->EODItemID);
		break;
	case 18:
		OwningEODPlayer->StopSkill(Skill_18->EODItemID);
		break;
	case 19:
		OwningEODPlayer->StopSkill(Skill_19->EODItemID);
		break;
	case 20:
		OwningEODPlayer->StopSkill(Skill_20->EODItemID);
		break;
	default:
		break;
	}
}

void USkillBarWidget::SetOwningEODPlayer(APlayerCharacter * NewOwner)
{
	OwningEODPlayer = NewOwner;
}

APlayerCharacter * USkillBarWidget::GetOwningEODPlayer() const
{
	return OwningEODPlayer;
}
