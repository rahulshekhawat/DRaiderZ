// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusIndicatorWidget.h"



UStatusIndicatorWidget::UStatusIndicatorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UStatusIndicatorWidget::Initialize()
{
	if (Super::Initialize() &&
		HealthBar &&
		HealthBarBlocked &&
		HealthValueText &&
		HealthOverlay &&
		ManaBar &&
		ManaValueText &&
		ManaBarBlocked &&
		ManaOverlay &&
		StaminaBar &&
		StaminaBarBlocked &&
		StaminaValueText &&
		StaminaOverlay)
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

void UStatusIndicatorWidget::UpdateHealthBar(int32 MaxHealth, int32 CurrentHealth)
{
	float CMHratio = (float)CurrentHealth / (float)MaxHealth;
	CMHratio = CMHratio > 1.0 ? 1.0 : CMHratio;

	if (HealthBar)
	{
		HealthBar->SetPercent(CMHratio);
	}

	FString HealthString = FString::FromInt(CurrentHealth) + FString("/") + FString::FromInt(MaxHealth);
	FText HealthText = FText::FromString(HealthString);
	HealthValueText->SetText(HealthText);
}

void UStatusIndicatorWidget::UpdateManaBar(int32 MaxMana, int32 CurrentMana)
{
	float CMMratio = (float)CurrentMana / (float)MaxMana;
	CMMratio = CMMratio > 1.0 ? 1.0 : CMMratio;

	if (ManaBar)
	{
		ManaBar->SetPercent(CMMratio);
	}

	FString ManaString = FString::FromInt(CurrentMana) + FString("/") + FString::FromInt(MaxMana);
	FText ManaText = FText::FromString(ManaString);
	ManaValueText->SetText(ManaText);
}

void UStatusIndicatorWidget::UpdateStaminaBar(int32 MaxStamina, int32 CurrentStamina)
{
	float CMSratio = (float)CurrentStamina / (float)MaxStamina;
	CMSratio = CMSratio > 1.0 ? 1.0 : CMSratio;

	if (StaminaBar)
	{
		StaminaBar->SetPercent(CMSratio);
	}

	FString StaminaString = FString::FromInt(CurrentStamina) + FString("/") + FString::FromInt(MaxStamina);
	FText StaminaText = FText::FromString(StaminaString);
	StaminaValueText->SetText(StaminaText);
}
