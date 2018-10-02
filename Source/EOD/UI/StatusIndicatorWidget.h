// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusIndicatorWidget.generated.h"

class UImage;
class USpacer;
class UOverlay;
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


	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProgressBar)
	// float Percent;

	FORCEINLINE void UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth);

	FORCEINLINE void UpdateManaBar(int32 CurrentMana, int32 MaxMana, int32 BaseMana);

	FORCEINLINE void UpdateStaminaBar(int32 CurrentStamina, int32 MaxStamina, int32 BaseStamina);

private:

	void SetHorizontalSlotSize(class UHorizontalBoxSlot* HBSlot, float FillValue);

	// FORCEINLINE SetHealthPercent(float Value);

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UProgressBar* HealthBar;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UProgressBar* ManaBar;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UProgressBar* StaminaBar;

	// @todo Class icon

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UTextBlock* LevelText;

	
	
	
};
