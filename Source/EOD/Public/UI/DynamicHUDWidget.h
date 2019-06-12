// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicHUDWidget.generated.h"

class UContainerWidget;
class USoundBase;
class UTextBlock;
class UCanvasPanel;
class UCanvasPanelSlot;
class UDynamicSkillBarWidget;
class UInventoryWidget;
class UPlayerStatsWidget;
class UStatusIndicatorWidget;
class UDialogueWindowWidget;
class UDynamicSkillTreeWidget;
class UNotificationWidget;
class UGameplayEffectBase;

/**
 * 
 */
UCLASS()
class EOD_API UDynamicHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UDynamicHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// --------------------------------------
	//	Child Widgets
	// --------------------------------------

	/** The main canvas that contains all the child widgets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	/** Widget containing health, mana, and stamina bars */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UStatusIndicatorWidget* StatusIndicatorWidget;

	/** Widget containing player stats info */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UPlayerStatsWidget* PlayerStatsWidget;

	/** Widget containing player inventory items */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	/** Widget containing skill trees of all vocations */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UDynamicSkillTreeWidget* SkillTreeWidget;

	/** Widget containing skills that can be used by player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UDynamicSkillBarWidget* SkillBarWidget;

public:

	FORCEINLINE UCanvasPanel* GetMainCanvas() const { return MainCanvas; }

	FORCEINLINE UStatusIndicatorWidget* GetStatusIndicatorWidget() const { return StatusIndicatorWidget; }

	FORCEINLINE UDynamicSkillBarWidget* GetSkillBarWidget() const { return SkillBarWidget; }

	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	FORCEINLINE UDynamicSkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

	FORCEINLINE UPlayerStatsWidget* GetPlayerStatsWidget() const { return PlayerStatsWidget; }

	/** Add skill bar widget as a child to HUD widget */
	void AddSkillBarWidget(UDynamicSkillBarWidget* NewWidget);

	/** Add dialogue widget as a child to HUD widget */
	void AddDialogueWidget(UDialogueWindowWidget* NewWidget);

	void AddGameplayEffectUI(UGameplayEffectBase* GameplayEffect);

	void RemoveGameplayEffectUI(UGameplayEffectBase* GameplayEffect);

private:

	// --------------------------------------
	//  Cache
	// --------------------------------------

	UPROPERTY(Transient)
	TMap<UGameplayEffectBase*, UContainerWidget*> GameplayEffectUIMap;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D DialogueWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D DialogueWidgetPosition;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SkillBarWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SkillBarWidgetPosition;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAnchors SkillBarWidgetAnchor;

	UPROPERTY(Category = Sounds, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USoundBase* NotificationSound;

	UPROPERTY(Category = Sounds, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UContainerWidget> GameplayEffectWidgetClass;

public:

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void DisplayNotification(UNotificationWidget* NotificationWidget);

	UFUNCTION()
	void RemoveNotification(UNotificationWidget* NotificationWidget);

protected:

	// TQueue<UNotificationWidget*> Notifications;
	UPROPERTY(Transient)
	TMap<UNotificationWidget*, UCanvasPanelSlot*> Notifications;

};
