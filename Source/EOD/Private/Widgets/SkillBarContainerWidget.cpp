// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillBarContainerWidget.h"
#include "PlayerSkillBase.h"
#include "TooltipWidget.h"
#include "GameplaySkillsComponent.h"
#include "EODGlobalNames.h"
#include "DragVisualWidget.h"
#include "EODDragDropOperation.h"
#include "SkillTreeContainerWidget.h"
#include "PlayerSkillsComponent.h"

#include "WidgetBlueprintLibrary.h"

USkillBarContainerWidget::USkillBarContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkillBarIndex = -1;
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

		UEODDragDropOperation* DragDropOp = Cast<UEODDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UEODDragDropOperation::StaticClass()));
		if (DragDropOp)
		{
			UDragVisualWidget* DragVisualWidget = CreateWidget<UDragVisualWidget>(GetOwningPlayer(), DragWidgetClass);
			DragVisualWidget->DragIcon = PlayerSkill->GetSkillIcon();

			DragDropOp->DragOpSourceContainer = this;
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
	UEODDragDropOperation* DragOp = Cast<UEODDragDropOperation>(InOperation);
	if (DragOp == nullptr)
	{
		return false;
	}

	USkillTreeContainerWidget* SourceSTCont = DragOp ? Cast<USkillTreeContainerWidget>(DragOp->DragOpSourceContainer) : nullptr;
	if (SourceSTCont != nullptr)
	{
		UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(DragOp->Payload);
		check(Skill);

		UPlayerSkillsComponent* SkillsComp = Cast<UPlayerSkillsComponent>(Skill->InstigatorSkillComponent.Get());
		check(SkillsComp);
		SkillsComp->AddSkillToSkillBar(SkillBarIndex, Skill->GetSkillGroup());
		SetDataObj(Skill);
		return true;
	}

	USkillBarContainerWidget* SourceSBCont = DragOp ? Cast<USkillBarContainerWidget>(DragOp->DragOpSourceContainer) : nullptr;
	if (SourceSBCont)
	{
		UPlayerSkillBase* SourceSkill = Cast<UPlayerSkillBase>(DragOp->Payload);
		check(SourceSkill);

		UPlayerSkillsComponent* SkillsComp = Cast<UPlayerSkillsComponent>(SourceSkill->InstigatorSkillComponent.Get());
		check(SkillsComp);

		UPlayerSkillBase* DestSkill = Cast<UPlayerSkillBase>(GetDataObj());

		SourceSBCont->SetDataObj(DestSkill);
		this->SetDataObj(SourceSkill);

		if (DestSkill)
		{
			SkillsComp->AddSkillToSkillBar(SourceSBCont->SkillBarIndex, DestSkill->GetSkillGroup());
		}
		else
		{
			SkillsComp->RemoveSkillFromSkillBar(SourceSBCont->SkillBarIndex, NAME_None);
		}

		SkillsComp->AddSkillToSkillBar(this->SkillBarIndex, SourceSkill->GetSkillGroup());

		return true;
	}

	return false;
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
	UPlayerSkillBase* OldSkill = Cast<UPlayerSkillBase>(GetDataObj());
	if (OldSkill)
	{
		OldSkill->UnlinkFromWidget(this);
	}


	UPlayerSkillBase* NewSkill = Cast<UPlayerSkillBase>(InDataObj);
	if (NewSkill)
	{
		DataObj = NewSkill;

		SetIcon(NewSkill->GetSkillIcon());
		SetSubText(NewSkill->GetCurrentUpgrade(), NewSkill->GetMaxUpgradeLevel());

		if (NewSkill->IsSkillInCooldown())
		{
			EnableCooldown();
			SetCooldownValue(NewSkill->GetRemainingCooldown());
		}
		else
		{
			DisableCooldown();
			SetCooldownValue(0);

		}

		UpdateTooltipWidget();

		NewSkill->LinkToWidget(this);
	}
	else
	{
		DataObj = nullptr;
		SetIcon(nullptr);
		DisableCooldown();
		EnableContainer();
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
