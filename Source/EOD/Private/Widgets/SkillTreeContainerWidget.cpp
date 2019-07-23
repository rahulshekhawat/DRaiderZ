// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeContainerWidget.h"
#include "PlayerSkillBase.h"

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

void USkillTreeContainerWidget::SetDataObj(UObject* InDataObj)
{
	UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(InDataObj);
	if (Skill)
	{
		DataObj = Skill;

		SetIcon(Skill->GetSkillIcon());
		SetSubText(Skill->GetCurrentUpgrade(), Skill->GetMaxUpgradeLevel());
		SetCooldown(Skill->GetRemainingCooldown());

		//~ @todo enable/disable skill

		UpdateTooltipWidget();
	}
}
