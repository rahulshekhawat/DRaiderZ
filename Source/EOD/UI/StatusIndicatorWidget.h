// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusIndicatorWidget.generated.h"

class UImage;
class UTextBlock;

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

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProgressBar)
	// float Percent;

	FORCEINLINE void UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth);

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
