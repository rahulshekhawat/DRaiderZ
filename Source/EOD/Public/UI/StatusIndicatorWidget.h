// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/UserWidget.h"
#include "StatusIndicatorWidget.generated.h"

class UImage;
class USpacer;
class UOverlay;
class UProgressBar;
class UHorizontalBoxSlot;

/**
 * 
 */
UCLASS()
class EOD_API UStatusIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UStatusIndicatorWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ManaBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* HealthBarBlocked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ManaBarBlocked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* StaminaBarBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* HealthOverlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* ManaOverlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* StaminaOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HealthValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ManaValueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StaminaValueText;

	// --------------------------------------
	//  Widget Behaviour
	// --------------------------------------

	UFUNCTION()
	void UpdateHealthBar(int32 MaxHealth, int32 CurrentHealth);

	UFUNCTION()
	void UpdateManaBar(int32 MaxMana, int32 CurrentMana);

	UFUNCTION()
	void UpdateStaminaBar(int32 MaxStamina, int32 CurrentStamina);

private:

	inline void SetHorizontalSlotSize(UHorizontalBoxSlot* HBSlot, float FillValue);

	inline void UpdateHealthBar_Internal(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth);
	inline void UpdateManaBar_Internal(int32 BaseMana, int32 MaxMana, int32 CurrentMana);
	inline void UpdateStaminaBar_Internal(int32 BaseStamina, int32 MaxStamina, int32 CurrentStamina);

};

inline void UStatusIndicatorWidget::SetHorizontalSlotSize(UHorizontalBoxSlot* HBSlot, float FillValue)
{
	if (IsValid(HBSlot))
	{
		FSlateChildSize StaminaOverlaySize;
		StaminaOverlaySize.SizeRule = ESlateSizeRule::Fill;
		StaminaOverlaySize.Value = FillValue;
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

	if (HealthBar)
	{
		HealthBar->SetPercent(CMHratio);
	}

	FString HealthString = FString::FromInt(CurrentHealth) + FString("/") + FString::FromInt(MaxHealth);
	FText HealthText = FText::FromString(HealthString);
	HealthValueText->SetText(HealthText);
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

	if (ManaBar)
	{
		ManaBar->SetPercent(CMMratio);
	}

	FString ManaString = FString::FromInt(CurrentMana) + FString("/") + FString::FromInt(MaxMana);
	FText ManaText = FText::FromString(ManaString);
	ManaValueText->SetText(ManaText);
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

	if (StaminaBar)
	{
		StaminaBar->SetPercent(CMSratio);
	}

	FString StaminaString = FString::FromInt(CurrentStamina) + FString("/") + FString::FromInt(MaxStamina);
	FText StaminaText = FText::FromString(StaminaString);
	StaminaValueText->SetText(StaminaText);
}
