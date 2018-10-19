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
class UCanvasPanel;

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

	/** Widget containing health, mana, and stamina bars */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UStatusIndicatorWidget* StatusIndicatorWidget;

	/** Widget containing skills that can be used by player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillBarWidget* SkillBarWidget;

	/** Widget containing player inventory items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	/** Widget containing skill trees of all vocations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USkillTreeWidget* SkillTreeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	/** Updates health bar in StatusIndicatorWidget */
	FORCEINLINE void UpdateHealthBar(int32 CurrentHealth, int32 MaxHealth, int32 BaseHealth);

	/** Updates mana bar in StatusIndicatorWidget */
	FORCEINLINE void UpdateManaBar(int32 CurrentMana, int32 MaxMana, int32 BaseMana);

	/** Updates stamina bar in StatusIndicatorWidget */
	FORCEINLINE void UpdateStaminaBar(int32 CurrentStamina, int32 MaxStamina, int32 BaseStamina);

	/** Get SkillID of skill placed at SkillIndex of skill bar */
	FORCEINLINE FName GetSkillAtIndex(int32 SkillIndex) const;

	/** Returns true if skill at given skill index of skill bar is in cooldown */
	FORCEINLINE bool IsSkillInCooldown(int32 SkillIndex) const;

	/** Put skill at given skill index of skill bar on cooldown */
	FORCEINLINE void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	/** Save current HUD layout to the current save slot */
	FORCEINLINE void SaveHUDLayout();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Skill Bar Widget To Canvas"))
	void BP_AddSkillBarWidgetToCanvas(USkillBarWidget* NewWidget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Skill Tree Widget To Canvas"))
	void BP_AddSkillTreeWidgetToCanvas(USkillBarWidget* NewWidget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Inventory Widget To Canvas"))
	void BP_AddInventoryWidgetToCanvas(USkillBarWidget* NewWidget);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Status Indicator Widget To Canvas"))
	void BP_AddStatusIndicatorWidgetToCanvas(USkillBarWidget* NewWidget);

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
