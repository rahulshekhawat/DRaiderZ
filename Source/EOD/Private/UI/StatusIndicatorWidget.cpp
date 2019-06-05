// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusIndicatorWidget.h"

UStatusIndicatorWidget::UStatusIndicatorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BarFillRate = 1.f;
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

void UStatusIndicatorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bHealthBarDirty)
	{
		UpdateHealthProgressBar(InDeltaTime);
	}

	if (bManaBarDirty)
	{
		UpdateManaProgressBar(InDeltaTime);
	}

	if (bStaminaBarDirty)
	{
		UpdateStaminaProgressBar(InDeltaTime);
	}
}

void UStatusIndicatorWidget::UpdateHealthBar(int32 MaxHealth, int32 CurrentHealth)
{
	Cached_MaxHealth = MaxHealth;
	Cached_CurrentHealth = CurrentHealth;

	bHealthBarDirty = true;

	if (HealthValueText)
	{
		FString HealthString = FString::FromInt(CurrentHealth) + FString("/") + FString::FromInt(MaxHealth);
		FText HealthText = FText::FromString(HealthString);
		HealthValueText->SetText(HealthText);
	}
}

void UStatusIndicatorWidget::UpdateManaBar(int32 MaxMana, int32 CurrentMana)
{
	Cached_MaxMana = MaxMana;
	Cached_CurrentMana = CurrentMana;

	bManaBarDirty = true;

	if (ManaValueText)
	{
		FString ManaString = FString::FromInt(CurrentMana) + FString("/") + FString::FromInt(MaxMana);
		FText ManaText = FText::FromString(ManaString);
		ManaValueText->SetText(ManaText);
	}
}

void UStatusIndicatorWidget::UpdateStaminaBar(int32 MaxStamina, int32 CurrentStamina)
{
	Cached_MaxStamina = MaxStamina;
	Cached_CurrentStamina = CurrentStamina;

	bStaminaBarDirty = true;

	if (StaminaValueText)
	{
		FString StaminaString = FString::FromInt(CurrentStamina) + FString("/") + FString::FromInt(MaxStamina);
		FText StaminaText = FText::FromString(StaminaString);
		StaminaValueText->SetText(StaminaText);
	}
}

void UStatusIndicatorWidget::UpdateHealthProgressBar(float DeltaTime)
{
	if (HealthBar)
	{
		float CMHratio = (float)Cached_CurrentHealth / (float)Cached_MaxHealth;
		CMHratio = CMHratio > 1.0 ? 1.0 : CMHratio;

		float DeltaChange = DeltaTime * BarFillRate;
		float CurrentValue = HealthBar->Percent;
		float DesiredValue = CMHratio;

		float ValueToSet = FMath::FixedTurn(CurrentValue, DesiredValue, DeltaChange);

		if (ValueToSet == DesiredValue)
		{
			bHealthBarDirty = false;
		}

		HealthBar->SetPercent(ValueToSet);
	}
}

void UStatusIndicatorWidget::UpdateManaProgressBar(float DeltaTime)
{
	if (ManaBar)
	{
		float CMMratio = (float)Cached_CurrentMana / (float)Cached_MaxMana;
		CMMratio = CMMratio > 1.0 ? 1.0 : CMMratio;

		float DeltaChange = DeltaTime * BarFillRate;
		float CurrentValue = ManaBar->Percent;
		float DesiredValue = CMMratio;

		float ValueToSet = FMath::FixedTurn(CurrentValue, DesiredValue, DeltaChange);

		if (ValueToSet == DesiredValue)
		{
			bManaBarDirty = false;
		}

		ManaBar->SetPercent(ValueToSet);
	}
}

void UStatusIndicatorWidget::UpdateStaminaProgressBar(float DeltaTime)
{
	if (StaminaBar)
	{
		float CMSratio = (float)Cached_CurrentStamina / (float)Cached_MaxStamina;
		CMSratio = CMSratio > 1.0 ? 1.0 : CMSratio;

		float DeltaChange = DeltaTime * BarFillRate;
		float CurrentValue = StaminaBar->Percent;
		float DesiredValue = CMSratio;

		float ValueToSet = FMath::FixedTurn(CurrentValue, DesiredValue, DeltaChange);

		if (ValueToSet == DesiredValue)
		{
			bStaminaBarDirty = false;
		}

		StaminaBar->SetPercent(ValueToSet);
	}
}
