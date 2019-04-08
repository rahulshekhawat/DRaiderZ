// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DamageNumberWidget.h"

#include "TextBlock.h"

UDamageNumberWidget::UDamageNumberWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDamageNumberWidget::Initialize()
{
	if (Super::Initialize() &&
		DamageText)
	{
		return true;
	}

	return false;
}

void UDamageNumberWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDamageNumberWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDamageNumberWidget::SetDamageValue(float Damage)
{
	FText Text = FText::FromString(FString::FromInt(Damage));
	if (DamageText)
	{
		DamageText->SetText(Text);
	}
}

void UDamageNumberWidget::SetDamageColor(FLinearColor NewColor)
{
	if (DamageText)
	{
		FSlateColor SlateColor(NewColor);
		DamageText->SetColorAndOpacity(SlateColor);
	}
}
