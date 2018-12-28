// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODItemContainer.h"
#include "EOD/UI/DragVisualWidget.h"
#include "EOD/Characters/PlayerCharacter.h"
#include "EOD/UI/EODItemDragDropOperation.h"

#include "Styling/SlateTypes.h"
#include "Engine/Texture.h"


UEODItemContainer::UEODItemContainer(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UEODItemContainer::Initialize()
{
	if (!(Super::Initialize() &&
		StackCountText &&
		CooldownText &&
		ItemImage))
	{
		return false;
	}

	StackCountText->SetVisibility(ESlateVisibility::Hidden);
	CooldownText->SetVisibility(ESlateVisibility::Hidden);

	SetupEmptyBorderMaterial();
	RefreshContainerVisuals();
	return true;
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

void UEODItemContainer::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	OnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UEODItemContainer::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	// Cannot drop anything on skill tree
	if (ContainerType == EEODContainerType::SkillTree)
	{
		return false;
	}

	UEODItemDragDropOperation* Operation = Cast<UEODItemDragDropOperation>(InOperation);
	if (!Operation || !Operation->DraggedEODItemWidget)
	{
		return false;
	}

	bool bResult = false;
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

	if (bResult && this->ContainerType == EEODContainerType::SkillBar)
	{
		APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwningPlayerPawn());
		// OwningPlayer->AddSkill(EODItemInfo.ItemID, EODItemInfo.StackCount);
	}

	// Cannot drop anything from skill tree to inventory
	// Cannot drop anything from inventory to skill bar
	// Cannot drop anything from skill bar to inventory

	return bResult;
}

void UEODItemContainer::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	UMaterialInstanceDynamic* DynamicMaterial = EmptyBorderImage->GetDynamicMaterial();
	DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), HoveredBorderColor);
}

void UEODItemContainer::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	UMaterialInstanceDynamic* DynamicMaterial = EmptyBorderImage->GetDynamicMaterial();
	DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), NormalBorderColor);
}

FReply UEODItemContainer::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	UMaterialInstanceDynamic* DynamicMaterial = EmptyBorderImage->GetDynamicMaterial();
	DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), PressedBorderColor);

	FReply Reply = FReply::Unhandled();
	FKey DragKey(TEXT("LeftMouseButton"));
	if (InMouseEvent.GetEffectingButton() == DragKey)
	{
		Reply = FReply::Handled();
		TSharedPtr<SWidget> SlateWidgetDetectingDrag = this->GetCachedWidget();
		if (SlateWidgetDetectingDrag.IsValid())
		{
			Reply = Reply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
		}
	}

	return Reply;
}

FReply UEODItemContainer::NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	UMaterialInstanceDynamic* DynamicMaterial = EmptyBorderImage->GetDynamicMaterial();
	DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), HoveredBorderColor);

	return FReply::Handled();
}

void UEODItemContainer::UpdateCooldown()
{
	if (CooldownTimeRemaining <= 0)
	{
		StopCooldown();
		return;
	}

	CooldownText->SetText(FText::FromString(FString::FromInt(CooldownTimeRemaining)));
	CooldownTimeRemaining -= CooldownInterval;
}
