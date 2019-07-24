// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UCanvasPanel;
class USkillBarWidget;
class UInventoryWidget;
class USkillTreeWidget;
class UPlayerStatsWidget;
class UGameplayEffectBase;
class UStatusIndicatorWidget;
class UInteractivePopupWidget;

/**
 * 
 */
UCLASS()
class EOD_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
public:

	inline UCanvasPanel* GetMainCanvas() const { return MainCanvas; }

	inline UStatusIndicatorWidget* GetStatusIndicatorWidget() const { return StatusIndicatorWidget; }

	inline UPlayerStatsWidget* GetPlayerStatsWidget() const { return PlayerStatsWidget; }

	inline UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	inline USkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

	inline USkillBarWidget* GetSkillBarWidget() const { return SkillBarWidget; }
	
	inline UInteractivePopupWidget* GetInteractivePopupWidget() const { return InteractivePopup; }

protected:
	
	/** The main canvas that contains all the child widgets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	/** Widget containing health, mana, and stamina bars */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UStatusIndicatorWidget* StatusIndicatorWidget;

	/** Widget containing player stats info */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UPlayerStatsWidget* PlayerStatsWidget;

	/** Widget containing player inventory items */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	/** Widget containing skill trees of all vocations */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeWidget* SkillTreeWidget;
	
	/** Widget containing skills that the player can actively use */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarWidget* SkillBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UInteractivePopupWidget* InteractivePopup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerName;


	///////////////////////////////////////////////////////////////////////////
	//  Mouse Events
protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:
	
	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetPlayerLevel(int32 Level);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetPlayerName(FString Name);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
	void AddGameplayEffectUI(UGameplayEffectBase* GameplayEffect);
	virtual void AddGameplayEffectUI_Implementation(UGameplayEffectBase* GameplayEffect);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
	void RemoveGameplayEffectUI(UGameplayEffectBase* GameplayEffect);
	virtual void RemoveGameplayEffectUI_Implementation(UGameplayEffectBase* GameplayEffect);


};
