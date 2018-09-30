// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UStatusIndicatorWidget;
class USkillBarWidget;
class UInventoryWidget;
class USkillTreeWidget;

/**
 * HUDWidget is used to display player HUD
 */
UCLASS()
class EOD_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UHUDWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;	

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LevelText;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UStatusIndicatorWidget* StatusIndicatorWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillBarWidget* SkillBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeWidget* SkillTreeWidget;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))


	FORCEINLINE void UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth);

	/** Get SkillID of skill placed at SkillIndex of skill bar */
	FORCEINLINE FName GetSkillAtIndex(int32 SkillIndex);

	FORCEINLINE void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	FORCEINLINE bool IsSkillInCooldown(int32 SkillIndex);

	// FORCEINLINE void SetCurrentHealthPercent();

	/** TextBlock to display current/max health */
	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock* HealthText;
	
	/** TextBlock to display current/max mana */
	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock* ManaText;
	
	/** TextBlock to display current/max stamina */
	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock* StaminaText;
	
	
};
