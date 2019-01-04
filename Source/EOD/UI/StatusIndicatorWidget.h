// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/Overlay.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/UserWidget.h"
#include "StatusIndicatorWidget.generated.h"

class UImage;
class USpacer;
class UOverlay;
class UHorizontalBoxSlot;

/**
 * 
 */
UCLASS()
class EOD_API UStatusIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UStatusIndicatorWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HealthBarEmpty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HealthBarFill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HealthBarBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* HealthOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpacer* HealthSpacer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ManaBarEmpty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ManaBarFill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ManaBarBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* ManaOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpacer* ManaSpacer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* StaminaBarEmpty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* StaminaBarFill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* StaminaBarBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* StaminaOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpacer* StaminaSpacer;

	UFUNCTION()
	void UpdateHealthBar(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth);

	UFUNCTION()
	void UpdateManaBar(int32 BaseMana, int32 MaxMana, int32 CurrentMana);

	UFUNCTION()
	void UpdateStaminaBar(int32 BaseStamina, int32 MaxStamina, int32 CurrentStamina);

private:
	inline void SetHorizontalSlotSize(UHorizontalBoxSlot* HBSlot, float FillValue);

	inline void UpdateHealthBar_Internal(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth);

	inline void UpdateManaBar_Internal(int32 BaseMana, int32 MaxMana, int32 CurrentMana);

	inline void UpdateStaminaBar_Internal(int32 BaseStamina, int32 MaxStamina, int32 CurrentStamina);

};

inline void UStatusIndicatorWidget::SetHorizontalSlotSize(UHorizontalBoxSlot* HBSlot, float FillValue)
{
	FSlateChildSize StaminaOverlaySize;
	StaminaOverlaySize.SizeRule = ESlateSizeRule::Fill;
	StaminaOverlaySize.Value = FillValue;
	if (IsValid(HBSlot))
	{
		HBSlot->SetSize(StaminaOverlaySize);
	}
}

inline void UStatusIndicatorWidget::UpdateHealthBar_Internal(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth)
{
	float MBHratio = (float)MaxHealth / (float)BaseHealth;
	MBHratio = MBHratio > 1.0 ? 1.0 : MBHratio;

	UHorizontalBoxSlot* HealthOverlayHSlot = IsValid(HealthOverlay) ? Cast<UHorizontalBoxSlot>(HealthOverlay->Slot) : nullptr;
	UHorizontalBoxSlot* HealthBlockedHSlot = IsValid(HealthBarBlocked) ? Cast<UHorizontalBoxSlot>(HealthBarBlocked->Slot) : nullptr;

	SetHorizontalSlotSize(HealthOverlayHSlot, MBHratio);
	SetHorizontalSlotSize(HealthBlockedHSlot, 1 - MBHratio);

	float CMHratio = (float)CurrentHealth / (float)MaxHealth;
	CMHratio = CMHratio > 1.0 ? 1.0 : CMHratio;

	UHorizontalBoxSlot* HealthBarFillHSlot = IsValid(HealthBarFill) ? Cast<UHorizontalBoxSlot>(HealthBarFill->Slot) : nullptr;
	UHorizontalBoxSlot* HealthSpacerHSlot = IsValid(HealthSpacer) ? Cast<UHorizontalBoxSlot>(HealthSpacer->Slot) : nullptr;

	SetHorizontalSlotSize(HealthBarFillHSlot, CMHratio);
	SetHorizontalSlotSize(HealthSpacerHSlot, 1 - CMHratio);
}

inline void UStatusIndicatorWidget::UpdateManaBar_Internal(int32 BaseMana, int32 MaxMana, int32 CurrentMana)
{
	float MBMratio = (float)MaxMana / (float)BaseMana;
	MBMratio = MBMratio > 1.0 ? 1.0 : MBMratio;

	UHorizontalBoxSlot* ManaOverlayHSlot = IsValid(ManaOverlay) ? Cast<UHorizontalBoxSlot>(ManaOverlay->Slot) : nullptr;
	UHorizontalBoxSlot* ManaBlockedHSlot = IsValid(ManaBarBlocked) ? Cast<UHorizontalBoxSlot>(ManaBarBlocked->Slot) : nullptr;

	SetHorizontalSlotSize(ManaOverlayHSlot, MBMratio);
	SetHorizontalSlotSize(ManaBlockedHSlot, 1 - MBMratio);

	float CMMratio = (float)CurrentMana / (float)MaxMana;
	CMMratio = CMMratio > 1.0 ? 1.0 : CMMratio;

	UHorizontalBoxSlot* ManaBarFillHSlot = IsValid(ManaBarFill) ? Cast<UHorizontalBoxSlot>(ManaBarFill->Slot) : nullptr;
	UHorizontalBoxSlot* ManaSpacerHSlot = IsValid(ManaSpacer) ? Cast<UHorizontalBoxSlot>(ManaSpacer->Slot) : nullptr;

	SetHorizontalSlotSize(ManaBarFillHSlot, CMMratio);
	SetHorizontalSlotSize(ManaSpacerHSlot, 1 - CMMratio);
}

inline void UStatusIndicatorWidget::UpdateStaminaBar_Internal(int32 BaseStamina, int32 MaxStamina, int32 CurrentStamina)
{
	float MBSratio = (float)MaxStamina / (float)BaseStamina;
	MBSratio = MBSratio > 1.0 ? 1.0 : MBSratio;

	UHorizontalBoxSlot* StaminaOverlayHSlot = IsValid(StaminaOverlay) ? Cast<UHorizontalBoxSlot>(StaminaOverlay->Slot) : nullptr;
	UHorizontalBoxSlot* StaminaBlockedHSlot = IsValid(StaminaBarBlocked) ? Cast<UHorizontalBoxSlot>(StaminaBarBlocked->Slot) : nullptr;

	SetHorizontalSlotSize(StaminaOverlayHSlot, MBSratio);
	SetHorizontalSlotSize(StaminaBlockedHSlot, 1 - MBSratio);

	float CMSratio = (float)CurrentStamina / (float)MaxStamina;
	CMSratio = CMSratio > 1.0 ? 1.0 : CMSratio;

	UHorizontalBoxSlot* StaminaBarFillHSlot = IsValid(StaminaBarFill) ? Cast<UHorizontalBoxSlot>(StaminaBarFill->Slot) : nullptr;
	UHorizontalBoxSlot* StaminaSpacerHSlot = IsValid(StaminaSpacer) ? Cast<UHorizontalBoxSlot>(StaminaSpacer->Slot) : nullptr;

	SetHorizontalSlotSize(StaminaBarFillHSlot, CMSratio);
	SetHorizontalSlotSize(StaminaSpacerHSlot, 1 - CMSratio);
}
