// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HUDWidget.h"

UHUDWidget::UHUDWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UHUDWidget::Initialize()
{

	if (Super::Initialize() && StatusIndicatorWidget && SkillBarWidget && InventoryWidget) // && HealthBar && ManaBar && StaminaBar && LevelText) // && HealthText && ManaText && StaminaText)
	{
		return true;
	}

	return false;
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FORCEINLINE void UHUDWidget::UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth)
{
	
}

/*
FORCEINLINE void UHUDWidget::SetCurrentHealthPercent()
{
}
*/
