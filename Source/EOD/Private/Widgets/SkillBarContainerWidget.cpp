// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillBarContainerWidget.h"
#include "PlayerSkillBase.h"
#include "TooltipWidget.h"
#include "GameplaySkillsComponent.h"

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
		DataObj = Skill;

		SetIcon(Skill->GetSkillIcon());
		SetSubText(Skill->GetCurrentUpgrade(), Skill->GetMaxUpgradeLevel());
		SetCooldown(Skill->GetRemainingCooldown());

		//~ @todo enable/disable skill

		UpdateTooltipWidget();

		Skill->LinkToWidget(this);
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

void USkillBarContainerWidget::MainButtonClicked()
{
	Super::MainButtonClicked();

	UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(GetDataObj());
	UGameplaySkillsComponent* SkillsComp = Skill ? Skill->InstigatorSkillComponent.Get() : nullptr;
	if (SkillsComp)
	{
		SkillsComp->TriggerSkill(Skill->GetSkillIndex(), Skill);
	}
}

void USkillBarContainerWidget::UpdateTooltipWidget()
{
	UTooltipWidget* TTWidget = Cast<UTooltipWidget>(ToolTipWidget);
	if (TTWidget && DataObj.Get())
	{
		UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(DataObj.Get());
		check(Skill);
		
		TTWidget->SetIcon(Skill->GetSkillIcon());
		TTWidget->SetTitle(Skill->GetInGameSkillName());
		TTWidget->SetSubTitle(TEXT("Active Skill"));
		TTWidget->SetDescription(Skill->GetInGameDescription());

	}
}
