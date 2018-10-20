// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODItemContainer.h"
#include "EODItemDragDropOperation.h"
#include "Player/PlayerCharacter.h"
#include "DragVisualWidget.h"

#include "Image.h"
#include "Engine/Texture.h"
#include "SlateTypes.h"
#include "TextBlock.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"


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

FORCEINLINE void UEODItemContainer::StartCooldown(float Duration, float Interval)
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObjectChecked(this);
	}

	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UEODItemContainer::UpdateCooldown, Interval, true, 0.f);

	CooldownTimeRemaining = Duration;
	CooldownInterval = Interval;
	bInCooldown = true;
	ItemImage->SetIsEnabled(false);
	CooldownText->SetVisibility(ESlateVisibility::Visible);
}

FORCEINLINE void UEODItemContainer::StopCooldown()
{
	UWorld* World = nullptr;

	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObjectChecked(this);
	}

	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(CooldownTimerHandle);

	bInCooldown = false;
	ItemImage->SetIsEnabled(true);
	CooldownText->SetVisibility(ESlateVisibility::Hidden);
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

FORCEINLINE void UEODItemContainer::ResetContainer()
{
	EODItemInfo 				= FEODItemInfo();
	EODItemInfo.StackCount 		= 0;
	bCanBeClicked 				= false;
	bCanBeDragged 				= false;
	bInCooldown 				= false;
	CooldownTimeRemaining 		= 0;
	CooldownInterval 			= 0;
	
	// @note can't and shouldn't reset container type
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

FORCEINLINE void UEODItemContainer::UpdateItemImage()
{
	if (EODItemInfo.Icon)
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.ImageType = ESlateBrushImageType::FullColor;
		SlateBrush.SetResourceObject(EODItemInfo.Icon);
		ItemImage->SetBrush(SlateBrush);
	}
	else
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		SlateBrush.ImageType = ESlateBrushImageType::NoImage;
		ItemImage->SetBrush(SlateBrush);
	}
}

FORCEINLINE void UEODItemContainer::UpdateStackCountText()
{
	if (EODItemInfo.StackCount > 1)
	{
		FText Text = FText::FromString(FString::FromInt(EODItemInfo.StackCount));
		StackCountText->SetText(Text);
		StackCountText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FText Text = FText::FromString(FString(""));
		StackCountText->SetText(Text);
		StackCountText->SetVisibility(ESlateVisibility::Hidden);
	}
}

FORCEINLINE void UEODItemContainer::SetupEmptyBorderMaterial()
{
	if (!EmptyBorderMaterial)
	{
		return;
	}

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(EmptyBorderMaterial, this);
	DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), NormalBorderColor);

	FSlateBrush SlateBrush;
	SlateBrush.ImageSize = FVector2D(64.0, 64.0);
	SlateBrush.DrawAs = ESlateBrushDrawType::Image;
	SlateBrush.ImageType = ESlateBrushImageType::FullColor;
	SlateBrush.SetResourceObject(DynamicMaterial);
	EmptyBorderImage->SetBrush(SlateBrush);
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

/*
void UEODItemContainer::UpdateItemButton()
{
	if (EODItemInfo.Icon)
	{
		FSlateBrush SlateBrush;
		// SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.ImageType = ESlateBrushImageType::FullColor;
		SlateBrush.SetResourceObject(EODItemInfo.Icon);

		FButtonStyle ButtonStyle;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		ButtonStyle.SetNormal(SlateBrush);
		SlateBrush.ImageSize = FVector2D(54.0, 54.0);
		ButtonStyle.SetHovered(SlateBrush);
		SlateBrush.ImageSize = FVector2D(50.0, 50.0);
		ButtonStyle.SetPressed(SlateBrush);

		ItemButton->SetStyle(ButtonStyle);
	}
	else
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		SlateBrush.ImageType = ESlateBrushImageType::NoImage;

		FButtonStyle ButtonStyle;
		ButtonStyle.SetNormal(SlateBrush);
		ButtonStyle.SetHovered (SlateBrush);
		ButtonStyle.SetPressed(SlateBrush);

		ItemButton->SetStyle(ButtonStyle);
	}

}
*/

