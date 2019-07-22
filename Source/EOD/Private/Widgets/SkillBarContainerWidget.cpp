// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillBarContainerWidget.h"
#include "PlayerSkillBase.h"

USkillBarContainerWidget::USkillBarContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillBarContainerWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void USkillBarContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBarContainerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillBarContainerWidget::SetDataObj(UObject* InDataObj)
{
	UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(InDataObj);
	if (Skill)
	{
		SetIcon(Skill->GetSkillIcon());
	}


	// Skill bar container widget
	/*
	if (Skill && Cont)
	{
		Cont->SetIcon(Skill->GetSkillIcon());
		Cont->SetInGameName(Skill->GetInGameSkillName());
		Cont->SetDescription(Skill->GetInGameDescription());
		Cont->SetMaxValue(Skill->GetMaxUpgradeLevel());
		Cont->SetItemID(Skill->GetSkillGroup());
		Cont->SetItemGroup(Skill->GetSkillGroup());

		if (Skill->CanPlayerActivateThisSkill())
		{
			Cont->ItemImage->SetIsEnabled(true);
			Cont->SetCanBeClicked(true);
		}
		else
		{
			Cont->ItemImage->SetIsEnabled(false);
			Cont->SetCanBeClicked(false);
		}
	}
	*/
}
