// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeEnvelopWidget.h"

USkillTreeEnvelopWidget::USkillTreeEnvelopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeEnvelopWidget::Initialize()
{
	if (Super::Initialize() &&
		SkillTreeWidget)
	{
		return true;
	} 

	return false;
}

void USkillTreeEnvelopWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeEnvelopWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
