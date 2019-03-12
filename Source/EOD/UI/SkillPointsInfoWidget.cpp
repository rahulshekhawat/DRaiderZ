// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillPointsInfoWidget.h"
#include "TextBlock.h"

USkillPointsInfoWidget::USkillPointsInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillPointsInfoWidget::Initialize()
{
	if (Super::Initialize() &&
		AvailableSkillPointsNumText &&
		UsedSkillPointsNumText &&
		AssassinPointsNumText &&
		BerserkerPointsNumText &&
		ClericPointsNumText &&
		DefenderPointsNumText &&
		SorcererPointsNumText)
	{
		return true;
	}

	return false;
}

void USkillPointsInfoWidget::NativeConstruct()
{
}

void USkillPointsInfoWidget::NativeDestruct()
{
}
