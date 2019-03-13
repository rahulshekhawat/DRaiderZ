// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSkillTreeWidget.h"

UDynamicSkillTreeWidget::UDynamicSkillTreeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDynamicSkillTreeWidget::Initialize()
{
	if (Super::Initialize() &&
		AssassinCanvas &&
		BerserkerCanvas &&
		ClericCanvas &&
		DefenderCanvas &&
		SorcererCanvas &&
		AssassinInfoCanvas &&
		BerserkerInfoCanvas &&
		ClericInfoCanvas &&
		DefenderInfoCanvas &&
		SorcererInfoCanvas)
	{
		return true;
	}

	return false;
}

void UDynamicSkillTreeWidget::NativeConstruct()
{
}

void UDynamicSkillTreeWidget::NativeDestruct()
{
}
