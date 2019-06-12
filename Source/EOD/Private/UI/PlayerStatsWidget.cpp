// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerStatsWidget.h"

#include "Components/RichTextBlock.h"

UPlayerStatsWidget::UPlayerStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UPlayerStatsWidget::Initialize()
{
	if (Super::Initialize() &&
		PAttValue &&
		PCritValue &&
		PDefValue &&
		MAttValue &&
		MCritValue &&
		MDefValue)
	{
		return true;
	}

	return false;
}

void UPlayerStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStatsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayerStatsWidget::UpdateHealth(int32 MaxHealth, int32 CurrentHealth)
{
	if (HealthValue)
	{
		FText Text = FText::FromString(FString::FromInt(CurrentHealth) + FString("/") + FString::FromInt(MaxHealth));
		HealthValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateMana(int32 MaxMana, int32 CurrentMana)
{
	if (ManaValue)
	{
		FText Text = FText::FromString(FString::FromInt(CurrentMana) + FString("/") + FString::FromInt(MaxMana));
		ManaValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateStamina(int32 MaxStamina, int32 CurrentStamina)
{
	if (StaminaValue)
	{
		FText Text = FText::FromString(FString::FromInt(CurrentStamina) + FString("/") + FString::FromInt(MaxStamina));
		StaminaValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdatePAtt(float Value)
{
	if (PAttValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::SanitizeFloat(Value) + FString("</>"));
		PAttValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdatePCrit(float Value)
{
	if (PCritValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::SanitizeFloat(Value) + FString("</>"));
		PCritValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdatePDef(float Value)
{
	if (PDefValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::SanitizeFloat(Value) + FString("</>"));
		PDefValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateMAtt(float Value)
{
	if (MAttValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::SanitizeFloat(Value) + FString("</>"));
		MAttValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateMCrit(float Value)
{
	if (MCritValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::SanitizeFloat(Value) + FString("</>"));
		MCritValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateMDef(float Value)
{
	if (MDefValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::SanitizeFloat(Value) + FString("</>"));
		MDefValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateLevel(int32 Value)
{
	if (LevelValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::FromInt(Value) + FString("</>"));
		LevelValue->SetText(Text);
	}
}

void UPlayerStatsWidget::UpdateEXP(int32 Value)
{
	if (EXPValue)
	{
		FText Text = FText::FromString(FString("<ADD>") + FString::FromInt(Value) + FString("</>"));
		EXPValue->SetText(Text);
	}
}
