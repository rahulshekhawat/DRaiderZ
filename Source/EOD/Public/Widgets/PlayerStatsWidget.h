// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

class URichTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPlayerStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Child Widgets
	// --------------------------------------
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* PlayerType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* LevelValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* EXPValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* HealthValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* ManaValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* StaminaValue;

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* PAttValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* PCritValue;

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* PDefValue;

	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* MAttValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* MCritValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	URichTextBlock* MDefValue;
	
	UFUNCTION()
	void UpdateHealth(int32 MaxHealth, int32 CurrentHealth);

	UFUNCTION()
	void UpdateMana(int32 MaxMana, int32 CurrentMana);

	UFUNCTION()
	void UpdateStamina(int32 MaxStamina, int32 CurrentStamina);

	UFUNCTION()
	void UpdatePAtt(float Value);
	
	UFUNCTION()
	void UpdatePCrit(float Value);
	
	UFUNCTION()
	void UpdatePDef(float Value);
	
	UFUNCTION()
	void UpdateMAtt(float Value);
	
	UFUNCTION()
	void UpdateMCrit(float Value);
	
	UFUNCTION()
	void UpdateMDef(float Value);
	
	UFUNCTION()
	void UpdateLevel(int32 Value);
	
	UFUNCTION()
	void UpdateEXP(int32 Value);

	UFUNCTION()
	void SetPlayerName(FString Name);

	UFUNCTION()
	void SetPlayerType(FString Type);

	
};
