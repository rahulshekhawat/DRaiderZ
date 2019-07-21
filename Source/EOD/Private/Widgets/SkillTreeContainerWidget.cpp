// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeContainerWidget.h"

USkillTreeContainerWidget::USkillTreeContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeContainerWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void USkillTreeContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeContainerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
