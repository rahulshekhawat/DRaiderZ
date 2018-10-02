// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "StatusIndicatorWidget.h"

#include "Image.h"
#include "Spacer.h"
#include "Overlay.h"
#include "HorizontalBox.h"
#include "HorizontalBoxSlot.h"

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

FORCEINLINE void UStatusIndicatorWidget::UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth)
{
	float MBHratio = (float)MaxHealth / (float)BaseHealth;
	MBHratio = MBHratio > 1.0 ? 1.0 : MBHratio;

	UHorizontalBoxSlot* HealthOverlayHSlot = Cast<UHorizontalBoxSlot>(HealthOverlay->Slot);
	UHorizontalBoxSlot* HealthBlockedHSlot = Cast<UHorizontalBoxSlot>(HealthBarBlocked->Slot);

	SetHorizontalSlotSize(HealthOverlayHSlot, MBHratio);
	SetHorizontalSlotSize(HealthBlockedHSlot, 1 - MBHratio);

	float CMHratio = (float)CurrentHealth / (float)MaxHealth;
	CMHratio = CMHratio > 1.0 ? 1.0 : CMHratio;

	UHorizontalBoxSlot* HealthBarFillHSlot = Cast<UHorizontalBoxSlot>(HealthBarFill->Slot);
	UHorizontalBoxSlot* HealthSpacerHSlot = Cast<UHorizontalBoxSlot>(HealthSpacer->Slot);

	SetHorizontalSlotSize(HealthBarFillHSlot, CMHratio);
	SetHorizontalSlotSize(HealthSpacerHSlot, 1 - CMHratio);
}

FORCEINLINE void UStatusIndicatorWidget::UpdateManaBar(int32 CurrentMana, int32 MaxMana, int32 BaseMana)
{
	float MBMratio = (float)MaxMana / (float)BaseMana;
	MBMratio = MBMratio > 1.0 ? 1.0 : MBMratio;

	UHorizontalBoxSlot* ManaOverlayHSlot = Cast<UHorizontalBoxSlot>(ManaOverlay->Slot);
	UHorizontalBoxSlot* ManaBlockedHSlot = Cast<UHorizontalBoxSlot>(ManaBarBlocked->Slot);

	SetHorizontalSlotSize(ManaOverlayHSlot, MBMratio);
	SetHorizontalSlotSize(ManaBlockedHSlot, 1 - MBMratio);

	float CMMratio = (float)CurrentMana / (float)MaxMana;
	CMMratio = CMMratio > 1.0 ? 1.0 : CMMratio;

	UHorizontalBoxSlot* ManaBarFillHSlot = Cast<UHorizontalBoxSlot>(ManaBarFill->Slot);
	UHorizontalBoxSlot* ManaSpacerHSlot = Cast<UHorizontalBoxSlot>(ManaSpacer->Slot);

	SetHorizontalSlotSize(ManaBarFillHSlot, CMMratio);
	SetHorizontalSlotSize(ManaSpacerHSlot, 1 - CMMratio);
}

FORCEINLINE void UStatusIndicatorWidget::UpdateStaminaBar(int32 CurrentStamina, int32 MaxStamina, int32 BaseStamina)
{
	float MBSratio = (float)MaxStamina / (float)BaseStamina;
	MBSratio = MBSratio > 1.0 ? 1.0 : MBSratio;

	UHorizontalBoxSlot* StaminaOverlayHSlot = Cast<UHorizontalBoxSlot>(StaminaOverlay->Slot);
	UHorizontalBoxSlot* StaminaBlockedHSlot = Cast<UHorizontalBoxSlot>(StaminaBarBlocked->Slot);

	SetHorizontalSlotSize(StaminaOverlayHSlot, MBSratio);
	SetHorizontalSlotSize(StaminaBlockedHSlot, 1 - MBSratio);

	float CMSratio = (float)CurrentStamina / (float)MaxStamina;
	CMSratio = CMSratio > 1.0 ? 1.0 : CMSratio;

	UHorizontalBoxSlot* StaminaBarFillHSlot = Cast<UHorizontalBoxSlot>(StaminaBarFill->Slot);
	UHorizontalBoxSlot* StaminaSpacerHSlot = Cast<UHorizontalBoxSlot>(StaminaSpacer->Slot);

	SetHorizontalSlotSize(StaminaBarFillHSlot, CMSratio);
	SetHorizontalSlotSize(StaminaSpacerHSlot, 1 - CMSratio);
}

void UStatusIndicatorWidget::SetHorizontalSlotSize(UHorizontalBoxSlot * HBSlot, float FillValue)
{
	FSlateChildSize StaminaOverlaySize;
	StaminaOverlaySize.SizeRule = ESlateSizeRule::Fill;
	StaminaOverlaySize.Value = FillValue;
	HBSlot->SetSize(StaminaOverlaySize);
}
