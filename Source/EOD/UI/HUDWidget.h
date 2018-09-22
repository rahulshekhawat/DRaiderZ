// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LevelText;

	
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
