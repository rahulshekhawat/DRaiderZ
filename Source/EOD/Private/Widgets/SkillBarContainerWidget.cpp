// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillBarContainerWidget.h"
#include "PlayerSkillBase.h"
#include "TooltipWidget.h"
#include "GameplaySkillsComponent.h"
#include "EODGlobalNames.h"
#include "DragVisualWidget.h"

#include "WidgetBlueprintLibrary.h"

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

void USkillBarContainerWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(GetDataObj());
	if (PlayerSkill)
	{
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
}

bool USkillBarContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}

void USkillBarContainerWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	check(MainButton);
	MainButton->SetRenderScale(FVector2D(0.95f, 0.95f));
}

void USkillBarContainerWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	check(MainButton);
	MainButton->SetRenderScale(FVector2D(1.f, 1.f));
}

FReply USkillBarContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bRMBDown = InMouseEvent.IsMouseButtonDown(FKey(KeyboardKeysNames::RightMouseButton));

	FReply Reply = FReply::Handled();
	if (bRMBDown)
	{
		UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(GetDataObj());
		if (Skill)
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

FReply USkillBarContainerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void USkillBarContainerWidget::SetDataObj(UObject* InDataObj)
{
	UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(InDataObj);
	if (Skill)
	{
		DataObj = Skill;

		SetIcon(Skill->GetSkillIcon());
		SetSubText(Skill->GetCurrentUpgrade(), Skill->GetMaxUpgradeLevel());
		SetCooldownValue(Skill->GetRemainingCooldown());

		//~ @todo enable/disable skill

		UpdateTooltipWidget();

		Skill->LinkToWidget(this);
	}
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
