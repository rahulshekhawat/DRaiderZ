// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HUDWidget.h"
#include "SkillBarWidget.h"
#include "SkillTreeWidget.h"
#include "InventoryWidget.h"

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
	
}

FORCEINLINE FName UHUDWidget::GetSkillAtIndex(int32 SkillIndex)
{
	return SkillBarWidget->GetSkillAtIndex(SkillIndex);
}

FORCEINLINE void UHUDWidget::PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval)
{
	SkillBarWidget->PutSkillOnCooldownTimer(SkillIndex, Duration, Interval);
}

bool UHUDWidget::IsSkillInCooldown(int32 SkillIndex)
{
	return SkillBarWidget->IsSkillInCooldown(SkillIndex);
}

/*
FORCEINLINE void UHUDWidget::SetCurrentHealthPercent()
{
}
*/
