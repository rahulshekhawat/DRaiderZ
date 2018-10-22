// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusIndicatorWidget.h"

UStatusIndicatorWidget::UStatusIndicatorWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UStatusIndicatorWidget::Initialize()
{
	if (Super::Initialize() &&
		HealthBarEmpty &&
		HealthBarFill &&
		HealthBarBlocked &&
		HealthOverlay &&
		HealthSpacer &&
		ManaBarEmpty &&
		ManaBarFill &&
		ManaBarBlocked &&
		ManaOverlay &&
		ManaSpacer &&
		StaminaBarEmpty &&
		StaminaBarFill &&
		StaminaBarBlocked &&
		StaminaOverlay &&
		StaminaSpacer)
	{
		return true;
	}

	return false;
}

void UStatusIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusIndicatorWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
