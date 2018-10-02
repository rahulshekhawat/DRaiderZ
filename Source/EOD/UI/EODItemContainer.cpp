// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODItemContainer.h"
#include "EODItemDragDropOperation.h"

#include "Image.h"
#include "Engine/Texture.h"
#include "SlateTypes.h"
#include "TextBlock.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"


UEODItemContainer::UEODItemContainer(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UEODItemContainer::Initialize()
{
	if (Super::Initialize() && Text_StackCount && Text_Cooldown && ItemImage)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Hidden);
		Text_Cooldown->SetVisibility(ESlateVisibility::Hidden);

		UpdateItemImage();
		return true;
	}

	return false;
}

void UEODItemContainer::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateItemImage();
	// UpdateItemButton();

}

void UEODItemContainer::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEODItemContainer::StartCooldown(float Duration, float Interval)
{
	if (!GEngine)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UEODItemContainer::UpdateCooldown, Interval, true, 0.f);

	CooldownTimeRemaining = Duration;
	CooldownInterval = Interval;
	bInCooldown = true;
	ItemImage->SetIsEnabled(false);
	Text_Cooldown->SetVisibility(ESlateVisibility::Visible);
}

void UEODItemContainer::StopCooldown()
{
	if (!GEngine)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(CooldownTimerHandle);

	bInCooldown = false;
	ItemImage->SetIsEnabled(true);
	Text_Cooldown->SetVisibility(ESlateVisibility::Hidden);
}

void UEODItemContainer::RefreshContainerVisuals()
{
	UpdateItemImage();
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

	if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillTree &&
		ContainerType == EEODContainerType::SkillBar)
	{
		EODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;
		RefreshContainerVisuals();
	}
	else if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::Inventory &&
		ContainerType == EEODContainerType::Inventory)
	{
		FEODItemInfo TempEODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;
		
		Operation->DraggedEODItemWidget->EODItemInfo = this->EODItemInfo;
		this->EODItemInfo = TempEODItemInfo;

		Operation->DraggedEODItemWidget->RefreshContainerVisuals();
		this->RefreshContainerVisuals();
	}
	else if (Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillBar &&
		ContainerType == EEODContainerType::SkillBar)
	{
		FEODItemInfo TempEODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;

		Operation->DraggedEODItemWidget->EODItemInfo = this->EODItemInfo;
		this->EODItemInfo = TempEODItemInfo;

		Operation->DraggedEODItemWidget->RefreshContainerVisuals();
		this->RefreshContainerVisuals();
	}

	// Cannot drop anything from skill tree to inventory
	// Cannot drop anything from inventory to skill bar
	// Cannot drop anything from skill bar to inventory

	/*
	if ((Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillTree &&
		ContainerType == EEODContainerType::Inventory) ||
		(Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::Inventory &&
		ContainerType == EEODContainerType::SkillBar) ||
		(Operation->DraggedEODItemWidget->ContainerType == EEODContainerType::SkillBar &&
		ContainerType == EEODContainerType::Inventory))
	{
		return false;
	}

	if (ContainerType == EEODContainerType::SkillBar)
	{

	}
	else if (ContainerType == EEODContainerType::Inventory)
	{

	}
	*/

	return false;
}

void UEODItemContainer::UpdateItemImage()
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

void UEODItemContainer::UpdateItemButton()
{
	if (EODItemInfo.Icon)
	{
		/*
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
		*/
	}
	else
	{
		/*
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		SlateBrush.ImageType = ESlateBrushImageType::NoImage;

		FButtonStyle ButtonStyle;
		ButtonStyle.SetNormal(SlateBrush);
		ButtonStyle.SetHovered (SlateBrush);
		ButtonStyle.SetPressed(SlateBrush);

		ItemButton->SetStyle(ButtonStyle);
		*/
	}

}

void UEODItemContainer::UpdateCooldown()
{
	if (CooldownTimeRemaining <= 0)
	{
		StopCooldown();
		return;
	}

	// Text_Cooldown->SetText(FText::FromString(FString::SanitizeFloat(CooldownTimeRemaining)));
	Text_Cooldown->SetText(FText::FromString(FString::FromInt(CooldownTimeRemaining)));
	CooldownTimeRemaining -= CooldownInterval;
}
