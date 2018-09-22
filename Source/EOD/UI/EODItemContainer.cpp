// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODItemContainer.h"
#include "TextBlock.h"
#include "Image.h"

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

		return true;
	}

	return false;
}

void UEODItemContainer::NativeConstruct()
{
	Super::NativeConstruct();

	// StartCooldown(10);
	
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

	UWorld* World = GEngine->GetWorldFromContextObject(this);
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

	UWorld* World = GEngine->GetWorldFromContextObject(this);
	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(CooldownTimerHandle);

	bInCooldown = false;
	ItemImage->SetIsEnabled(true);
	Text_Cooldown->SetVisibility(ESlateVisibility::Hidden);
}

void UEODItemContainer::UpdateItemImage()
{
	if (EODItemIcon)
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(52.0, 52.0);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.ImageType = ESlateBrushImageType::FullColor;
		SlateBrush.SetResourceObject(EODItemIcon);
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
