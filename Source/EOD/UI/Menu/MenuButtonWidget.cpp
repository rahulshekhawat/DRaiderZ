// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MenuButtonWidget.h"

UMenuButtonWidget::UMenuButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UMenuButtonWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UMenuButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMenuButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
