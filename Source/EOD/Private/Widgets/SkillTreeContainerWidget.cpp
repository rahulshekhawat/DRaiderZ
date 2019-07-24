// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeContainerWidget.h"
#include "PlayerSkillBase.h"

USkillTreeContainerWidget::USkillTreeContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillTreeContainerWidget::Initialize()
{
	if (Super::Initialize() &&
		UpgradeButton)
	{
		// Skill Tree Container's Main Button can never be clicked
		MainButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		UpgradeButton->OnClicked.AddUniqueDynamic(this, &USkillTreeContainerWidget::UpgradeButtonClicked);
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

void USkillTreeContainerWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{

}

bool USkillTreeContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void USkillTreeContainerWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}

void USkillTreeContainerWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

FReply USkillTreeContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply();
}

FReply USkillTreeContainerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply();
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

		Skill->LinkToWidget(this);
	}
}

void USkillTreeContainerWidget::UpgradeButtonClicked()
{
	OnUpgradeButtonClicked.Broadcast(this);
}
