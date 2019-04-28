// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusIndicatorWidget.h"

UStatusIndicatorWidget::UStatusIndicatorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

void UStatusIndicatorWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UStatusIndicatorWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusIndicatorWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UStatusIndicatorWidget::UpdateHealthBar(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth)
{
	UpdateHealthBar_Internal(BaseHealth, MaxHealth, CurrentHealth);
}

void UStatusIndicatorWidget::UpdateManaBar(int32 BaseMana, int32 MaxMana, int32 CurrentMana)
{
	UpdateManaBar_Internal(BaseMana, MaxMana, CurrentMana);
}

void UStatusIndicatorWidget::UpdateStaminaBar(int32 BaseStamina, int32 MaxStamina, int32 CurrentStamina)
{
	UpdateStaminaBar_Internal(BaseStamina, MaxStamina, CurrentStamina);
}
