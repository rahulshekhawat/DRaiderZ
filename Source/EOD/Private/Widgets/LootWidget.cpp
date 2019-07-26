// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "LootWidget.h"

ULootWidget::ULootWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool ULootWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void ULootWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULootWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULootWidget::PickAllLoot_Implementation()
{
}

void ULootWidget::AddItemList_Implementation(const TArray<FGeneratedLootInfo>& LootInfoArray)
{
}

void ULootWidget::BP_SetLootSource(UObject* InLootSource)
{
	LootSource = InLootSource;
}

UObject* ULootWidget::BP_GetLootSource() const
{
	return LootSource.Get();
}
