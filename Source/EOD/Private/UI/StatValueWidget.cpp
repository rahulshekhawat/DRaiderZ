// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "StatValueWidget.h"

#include "Components/RichTextBlock.h"

UStatValueWidget::UStatValueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	StatName = FText::FromString(TEXT("Stat"));
	StatValue = FText::FromString(TEXT("<ADD>1</>"));
}

bool UStatValueWidget::Initialize()
{
	if (Super::Initialize() &&
		StatNameTextBlock &&
		StatValueTextBlock)
	{
		StatNameTextBlock->SetText(StatName);
		StatValueTextBlock->SetText(StatValue);
		return true;
	}

	return false;
}

void UStatValueWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatValueWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UStatValueWidget::SetStatName(const FString& InValue)
{
	check(StatNameTextBlock);
	StatName = FText::FromString(InValue);
	StatNameTextBlock->SetText(StatName);
}

void UStatValueWidget::SetStatValue(const FString& InValue, bool bAdd)
{
	check(StatValueTextBlock);
	if (bAdd)
	{
		StatValue = FText::FromString(TEXT("<ADD>") + InValue + TEXT("</>"));
	}
	else
	{
		StatValue = FText::FromString(TEXT("<RED>") + InValue + TEXT("</>"));
	}
	StatValueTextBlock->SetText(StatValue);
}
