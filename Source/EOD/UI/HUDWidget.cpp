// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HUDWidget.h"
#include "SkillBarWidget.h"
#include "SkillTreeWidget.h"
#include "InventoryWidget.h"
#include "StatusIndicatorWidget.h"

UHUDWidget::UHUDWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UHUDWidget::Initialize()
{

	if (Super::Initialize() && StatusIndicatorWidget && SkillBarWidget && InventoryWidget && SkillTreeWidget) // && HealthBar && ManaBar && StaminaBar && LevelText) // && HealthText && ManaText && StaminaText)
	{
		return true;
	}

	return false;
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FORCEINLINE void UHUDWidget::UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth)
{
	StatusIndicatorWidget->UpdateHealthBar(CurrentHealth, MaxHealth, BaseHealth);
}

FORCEINLINE void UHUDWidget::UpdateManaBar(int32 CurrentMana, int32 MaxMana, int32 BaseMana)
{
	StatusIndicatorWidget->UpdateManaBar(CurrentMana, MaxMana, BaseMana);
}

FORCEINLINE void UHUDWidget::UpdateStaminaBar(int32 CurrentStamina, int32 MaxStamina, int32 BaseStamina)
{
	StatusIndicatorWidget->UpdateStaminaBar(CurrentStamina, MaxStamina, BaseStamina);
}

FORCEINLINE FName UHUDWidget::GetSkillAtIndex(int32 SkillIndex) const
{
	return SkillBarWidget->GetSkillAtIndex(SkillIndex);
}

bool UHUDWidget::IsSkillInCooldown(int32 SkillIndex) const
{
	return SkillBarWidget->IsSkillInCooldown(SkillIndex);
}

FORCEINLINE void UHUDWidget::PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval)
{
	SkillBarWidget->PutSkillOnCooldownTimer(SkillIndex, Duration, Interval);
}

FORCEINLINE void UHUDWidget::SaveHUDLayout()
{
	SkillBarWidget->SaveSkillBarLayout();
}
