// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODItemContainer.h"
#include "TextBlock.h"

UEODItemContainer::UEODItemContainer(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UEODItemContainer::Initialize()
{
	if (Super::Initialize() && Text_StackCount && Text_Cooldown)
	{
		// Text_StackCount->SetVisibility(ESlateVisibility::Hidden);

		return true;
	}

	return false;
}

void UEODItemContainer::NativeConstruct()
{
	Super::NativeConstruct();

	// Text_StackCount->SetVisibility(ESlateVisibility::Hidden);
	// Text_Cooldown->SetVisibility(ESlateVisibility::Hidden);

	// Text_StackCount->SetText()

	StartCooldown(10);
	
}

void UEODItemContainer::NativeDestruct()
{
	Super::NativeDestruct();
}
