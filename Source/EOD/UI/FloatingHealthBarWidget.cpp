// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "FloatingHealthBarWidget.h"

UFloatingHealthBarWidget::UFloatingHealthBarWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UFloatingHealthBarWidget::Initialize()
{
	if (Super::Initialize() && HealthBar)
	{
		return true;
	}

	return false;
}

void UFloatingHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFloatingHealthBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
