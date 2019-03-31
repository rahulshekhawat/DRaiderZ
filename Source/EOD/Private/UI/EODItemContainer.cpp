// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODItemContainer.h"
#include "EODPreprocessors.h"
#include "PlayerCharacter.h"
#include "SkillBarWidget.h"
#include "DragVisualWidget.h"
#include "EODItemDragDropOperation.h"


#include "Styling/SlateTypes.h"
#include "Engine/Texture.h"


UEODItemContainer::UEODItemContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCanBeClicked = true;
}

bool UEODItemContainer::Initialize()
{
	if (Super::Initialize() && StackCountText && CooldownText && ItemImage)
	{
		StackCountText->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);

		SetupEmptyBorderMaterial();
		EnableContainer();
		return true;
	}
	return false;
}

void UEODItemContainer::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshContainerVisuals();
}

void UEODItemContainer::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEODItemContainer::BP_StartCooldown(float Duration, float Interval)
{
	StartCooldown(Duration, Interval);
}

void UEODItemContainer::BP_StopCooldown()
{
	StopCooldown();
}

void UEODItemContainer::RefreshContainerVisuals()
{
	UpdateItemImage();
	UpdateStackCountText();
}

void UEODItemContainer::AddSkill(FString& SkillGroup)
{
#if EOD_TEST_CODE_ENABLED
	// AEODCharacterBase* PlayerChar = GetOwningPlayer() ? Cast<AEODCharacterBase>(GetOwningPlayer()->GetPawn()) : nullptr;
	APlayerCharacter* PlayerChar = GetOwningPlayer() ? Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn()) : nullptr;
	if (IsValid(PlayerChar))
	{
		FString SkillIDString = PlayerChar->GetGenderPrefix() + SkillGroup + FString("_") + FString::FromInt(1);
		FName SkillID = FName(*SkillIDString);
		FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, FString("UEODItemContainer::AddSkill()"));
		if (Skill)
		{
			EODItemInfo.Description = Skill->Description;
			EODItemInfo.EODItemType = EEODItemType::ActiveSkill;
			EODItemInfo.Icon = Skill->Icon;
			EODItemInfo.InGameName = Skill->InGameName;
			EODItemInfo.ItemGroup = SkillGroup;
			EODItemInfo.ItemID = SkillID;
			EODItemInfo.StackCount = 1;

			RefreshContainerVisuals();
		}
	}
#endif // EOD_TEST_CODE_ENABLED
}

void UEODItemContainer::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UEODItemContainer::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Cannot drop anything on skill tree
	if (ContainerType == EEODContainerType::SkillTree)
	{
		return false;
	}

	UEODItemDragDropOperation* Operation = Cast<UEODItemDragDropOperation>(InOperation);
	if (!IsValid(Operation) || !IsValid(Operation->DraggedEODItemWidget))
	{
		return false;
	}


	bool bResult = false;
	// Cannot drop anything from skill tree to inventory
	// Cannot drop anything from inventory to skill bar
	// Cannot drop anything from skill bar to inventory
	if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillTree &&
		ContainerType == EEODContainerType::SkillBar)
	{
		USkillBarWidget* SkillBarWidget = Cast<USkillBarWidget>(ParentWidget);
		if (IsValid(SkillBarWidget))
		{
			bResult = SkillBarWidget->OnNewSkillDropped(Operation->DraggedEODItemWidget, this);
		}
	}
	else if ((Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::Inventory &&
		ContainerType == EEODContainerType::Inventory) ||
		(Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillBar &&
			ContainerType == EEODContainerType::SkillBar))
	{
		USkillBarWidget* SkillBarWidget = Cast<USkillBarWidget>(ParentWidget);
		if (IsValid(SkillBarWidget))
		{
			bResult = SkillBarWidget->OnSkillsSwapped(Operation->DraggedEODItemWidget, this);
		}
	}

	/*
	if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillTree &&
		ContainerType == EEODContainerType::SkillBar)
	{
		this->EODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;
		this->RefreshContainerVisuals();
		bResult = true;
	}
	else if ((Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::Inventory &&
		ContainerType == EEODContainerType::Inventory) ||
		(Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillBar &&
			ContainerType == EEODContainerType::SkillBar))
	{
		FEODItemInfo TempEODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;

		Operation->DraggedEODItemWidget->EODItemInfo = this->EODItemInfo;
		this->EODItemInfo = TempEODItemInfo;

		Operation->DraggedEODItemWidget->RefreshContainerVisuals();
		this->RefreshContainerVisuals();

		bResult = true;
	}
	*/

	return bResult;
}

void UEODItemContainer::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if ((bCanBeClicked || bCanBeDragged) && IsValid(EmptyBorderMID))
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, HoveredBorderColor);
	}
}

void UEODItemContainer::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (IsValid(EmptyBorderMID))
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
	}
}

FReply UEODItemContainer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();
	if (bCanBeClicked)
	{
		if (IsValid(EmptyBorderMID))
		{
			EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, PressedBorderColor);
		}

		if (bCanBeDragged)
		{
			FKey DragKey(KeyboardKeysNames::LeftMouseButton);
			if (InMouseEvent.GetEffectingButton() == DragKey)
			{
				TSharedPtr<SWidget> SlateWidgetDetectingDrag = this->GetCachedWidget();
				if (SlateWidgetDetectingDrag.IsValid())
				{
					Reply = Reply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				}
			}
		}
	}
	return Reply;
}

FReply UEODItemContainer::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if ((bCanBeClicked || bCanBeDragged) && IsValid(EmptyBorderMID))
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, HoveredBorderColor);
	}
	return FReply::Handled();
}

void UEODItemContainer::UpdateCooldown()
{
	if (CooldownTimeRemaining <= 0)
	{
		StopCooldown();
		return;
	}

	if (IsValid(CooldownText))
	{
		CooldownText->SetText(FText::FromString(FString::FromInt(CooldownTimeRemaining)));
	}
	CooldownTimeRemaining -= CooldownInterval;
}
