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
	Super::NativeConstruct();
}

void USkillPointsInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillPointsInfoWidget::UpdateAvailableSkillPointsText(int32 Points)
{
	if (Points != Cache_AvailableSkillPoints && AvailableSkillPointsNumText)
	{
		Cache_AvailableSkillPoints = Points;
		AvailableSkillPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}

void USkillPointsInfoWidget::UpdateUsedSkillPointsText(int32 Points)
{
	if (Points != Cache_UsedSkillPoints && UsedSkillPointsNumText)
	{
		Cache_UsedSkillPoints = Points;
		UsedSkillPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}

void USkillPointsInfoWidget::UpdateAssassinPointsText(int32 Points)
{
	if (Points != Cache_AssassinPoints && AssassinPointsNumText)
	{
		Cache_AssassinPoints = Points;
		AssassinPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}

void USkillPointsInfoWidget::UpdateBerserkerPointsText(int32 Points)
{
	if (Points != Cache_BerserkerPoints && BerserkerPointsNumText)
	{
		Cache_BerserkerPoints = Points;
		BerserkerPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}

void USkillPointsInfoWidget::UpdateClericPointsText(int32 Points)
{
	if (Points != Cache_ClericPoints && ClericPointsNumText)
	{
		Cache_ClericPoints = Points;
		ClericPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}

void USkillPointsInfoWidget::UpdateDefenderPointsText(int32 Points)
{
	if (Points != Cache_DefenderPoints && DefenderPointsNumText)
	{
		Cache_DefenderPoints = Points;
		DefenderPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}

void USkillPointsInfoWidget::UpdateSorcererPointsText(int32 Points)
{
	if (Points != Cache_SorcererPoints && SorcererPointsNumText)
	{
		Cache_SorcererPoints = Points;
		SorcererPointsNumText->SetText(FText::FromString(FString::FromInt(Points)));
	}
}
