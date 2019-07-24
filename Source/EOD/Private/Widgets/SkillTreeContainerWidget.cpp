// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeContainerWidget.h"
#include "PlayerSkillBase.h"
#include "DragVisualWidget.h"
#include "EODGlobalNames.h"

#include "WidgetBlueprintLibrary.h"

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
	// OnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// Skill tree container CANNOT be empty
	UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(GetDataObj());
	check(PlayerSkill);

	UClass* DragWidgetClass = DragVisualClass.Get();
	check(DragWidgetClass);

	UDragDropOperation* DragDropOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragDropOp)
	{
		UDragVisualWidget* DragVisualWidget = CreateWidget<UDragVisualWidget>(GetOwningPlayer(), DragWidgetClass);
		DragVisualWidget->DragIcon = PlayerSkill->GetSkillIcon();

		DragDropOp->DefaultDragVisual = DragVisualWidget;
		DragDropOp->Payload = GetDataObj();
		DragDropOp->Pivot = EDragPivot::CenterCenter;
		DragDropOp->Offset = FVector2D(0.f, 0.f);
	}

	OutOperation = DragDropOp;
}

bool USkillTreeContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// return OnDrop(InGeometry, InDragDropEvent, InOperation);
	
	// You can't drop anything on a skill tree container
	return false;
}

void USkillTreeContainerWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// OnMouseEnter(InGeometry, InMouseEvent);

	//~ Skill tree container doesn't react to mouse enter or leave
}

void USkillTreeContainerWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	// OnMouseLeave(InMouseEvent);

	//~ Skill tree container doesn't react to mouse enter or leave
}

FReply USkillTreeContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// return OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;

	bool bRMBDown = InMouseEvent.IsMouseButtonDown(FKey(KeyboardKeysNames::RightMouseButton));

	FReply Reply = FReply::Handled();
	if (bRMBDown)
	{
		UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(GetDataObj());
		check(Skill);

		if (Skill->IsUnlocked())
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = this->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply = Reply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), InMouseEvent.GetEffectingButton());
			}
		}
	}

	return Reply;
}

FReply USkillTreeContainerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// return OnMouseButtonUp(InGeometry, InMouseEvent).NativeReply;

	//~ Skill tree container doesnt' react to mouse button up
	return FReply::Handled();
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

void USkillTreeContainerWidget::EnableUpgradeButton()
{
	check(UpgradeButton);
	UpgradeButton->SetVisibility(ESlateVisibility::Visible);
}

void USkillTreeContainerWidget::DisableUpgradeButton()
{
	check(UpgradeButton);
	UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
}

void USkillTreeContainerWidget::UpgradeButtonClicked()
{
	OnUpgradeButtonClicked.Broadcast(this);
}
