// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AdvancedOptionsWidget.h"

UAdvancedOptionsWidget::UAdvancedOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAdvancedOptionsWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UAdvancedOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAdvancedOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
