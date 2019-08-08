// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillTreeContainerWidget.h"
#include "PlayerSkillBase.h"
#include "DragVisualWidget.h"
#include "EODGlobalNames.h"
#include "PlayerSkillsComponent.h"
#include "EODDragDropOperation.h"
#include "TooltipWidget.h"
#include "ActiveSkillBase.h"
#include "PlayerSkillBase.h"

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
		SubText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

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

bool USkillTreeContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// return OnDrop(InGeometry, InDragDropEvent, InOperation);
	
	// You can't drop anything on a skill tree container
	return true;
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
	check(InDataObj);

	UPlayerSkillBase* OldSkill = Cast<UPlayerSkillBase>(GetDataObj());
	if (OldSkill == InDataObj)
	{
		if (OldSkill->IsUnlocked())
		{
			EnableContainer();
		}
		else
		{
			DisableContainer();
		}

		SetSubText(OldSkill->GetCurrentUpgrade(), OldSkill->GetMaxUpgradeLevel());
		UpdateTooltipWidget();
		return;
	}

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
		SetCooldownValue(NewSkill->GetRemainingCooldown());

		if (NewSkill->IsUnlocked())
		{
			EnableContainer();
		}
		else
		{
			DisableContainer();
		}

		UpdateTooltipWidget();
		NewSkill->LinkToWidget(this);
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

void USkillTreeContainerWidget::EnableContainer()
{
	check(ItemImage);
	ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

	//~ Skill tree container's main button is always disabled
	check(MainButton);
	MainButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void USkillTreeContainerWidget::DisableContainer()
{
	check(ItemImage);
	ItemImage->SetColorAndOpacity(FLinearColor(0.15f, 0.15f, 0.15f, 1.f));

	check(MainButton);
	MainButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void USkillTreeContainerWidget::EnableCooldown()
{
	//~ intentional empty override
}

void USkillTreeContainerWidget::DisableCooldown()
{
	//~ intentional empty override
}

void USkillTreeContainerWidget::SetCooldownValue(float InValue)
{
	//~ intentional empty override
}

void USkillTreeContainerWidget::SetCurrentValue(int32 InValue)
{
	UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(GetDataObj());
	check(Skill);
	SetSubText(InValue, Skill->GetMaxUpgradeLevel());
}

void USkillTreeContainerWidget::UpdateTooltipWidget()
{
	UTooltipWidget* TTWidget = Cast<UTooltipWidget>(ToolTipWidget);
	if (TTWidget && DataObj.Get())
	{
		UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(DataObj.Get());
		check(Skill);

		TTWidget->SetIcon(Skill->GetSkillIcon());
		TTWidget->SetTitle(Skill->GetInGameSkillName());
		TTWidget->SetSubTitle(TEXT("Active Skill"));

		const FString& InGameDesc = Skill->GetInGameDescription();
		if (InGameDesc == TEXT(""))
		{
			TTWidget->SetDescription(TEXT("Description unavailable"));
		}
		else
		{
			TTWidget->SetDescription(InGameDesc);
		}

		UActiveSkillBase* ActiveSkill = Cast<UActiveSkillBase>(Skill);
		if (ActiveSkill)
		{
			const FActiveSkillLevelUpInfo SkillInfo = ActiveSkill->GetCurrentSkillLevelupInfo();

			if (SkillInfo.ManaCost > 0)
			{
				FString ManaStr = FString::FromInt(SkillInfo.ManaCost);
				TTWidget->AddStat(TEXT("Mana Cost"), ManaStr);
			}

			if (SkillInfo.StaminaCost > 0)
			{
				FString StaminaStr = FString::FromInt(SkillInfo.StaminaCost);
				TTWidget->AddStat(TEXT("Stamina Cost"), StaminaStr);
			}

			FString ValueStr = FString::SanitizeFloat(SkillInfo.Cooldown, 0) + TEXT("s");
			TTWidget->AddStat(TEXT("Cooldown"), ValueStr);

		}
	}
}

void USkillTreeContainerWidget::UpgradeButtonClicked()
{
	OnUpgradeButtonClicked.Broadcast(this);
}
