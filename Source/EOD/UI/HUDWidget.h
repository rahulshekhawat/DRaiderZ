// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EOD/UI/EODItemContainer.h"
#include "EOD/UI/SkillBarWidget.h"
#include "EOD/UI/SkillTreeWidget.h"
#include "EOD/UI/InventoryWidget.h"
#include "EOD/UI/PlayerStatsWidget.h"
#include "EOD/UI/DialogueWindowWidget.h"
#include "EOD/UI/StatusIndicatorWidget.h"

#include "UObject/ObjectMacros.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "HUDWidget.generated.h"

/**
 * HUDWidget is used to display player HUD
 */
UCLASS()
class EOD_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;	

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Child Widgets
	// --------------------------------------

	/** Widget containing health, mana, and stamina bars */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UStatusIndicatorWidget* StatusIndicatorWidget;

	/** Widget containing skills that can be used by player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarWidget* SkillBarWidget;

	/** Widget containing player inventory items */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	/** Widget containing skill trees of all vocations */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillTreeWidget* SkillTreeWidget;

	/** Widget containing player stats info */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UPlayerStatsWidget* PlayerStatsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LevelText;

public:
	FORCEINLINE UStatusIndicatorWidget* GetStatusIndicatorWidget() const;

	FORCEINLINE USkillBarWidget* GetSkillBarWidget() const;

	FORCEINLINE UInventoryWidget* GetInventoryWidget() const;

	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const;

	FORCEINLINE UPlayerStatsWidget* GetPlayerStatsWidget() const;

	FORCEINLINE UCanvasPanel* GetMainCanvas() const;

public:
	/** Save current HUD layout to the current save slot */
	void SaveHUDLayout();

	/** Add skill bar widget as a child to HUD widget */
	inline void AddSkillBarWidget(USkillBarWidget* NewWidget);

	/** Add skill tree widget as a child to HUD widget */
	inline void AddSkillTreeWidget(USkillTreeWidget* NewWidget);

	/** Add inventory widget as a child to HUD widget */
	inline void AddInventoryWidget(UInventoryWidget* NewWidget);

	/** Add dialogue widget as a child to HUD widget */
	inline void AddDialogueWidget(UDialogueWindowWidget* NewWidget);

	/** Add status indicator widget as a child to HUD widget */
	inline void AddStatusIndicatorWidget(UStatusIndicatorWidget* NewWidget);

	/** Add skill bar widget as a child to HUD widget */
	UFUNCTION(BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Skill Bar Widget To Canvas"))
	void BP_AddSkillBarWidgetToCanvas(USkillBarWidget* NewWidget);

	/** Add skill tree widget as a child to HUD widget */
	UFUNCTION(BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Skill Tree Widget To Canvas"))
	void BP_AddSkillTreeWidgetToCanvas(USkillTreeWidget* NewWidget);

	/** Add inventory widget as a child to HUD widget */
	UFUNCTION(BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Inventory Widget To Canvas"))
	void BP_AddInventoryWidgetToCanvas(UInventoryWidget* NewWidget);

	/** Add dialogue widget as a child to HUD widget */
	UFUNCTION(BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Dialogue Widget To Canvas"))
	void BP_AddDialogueWidgetToCanvas(UDialogueWindowWidget* NewWidget);

	/** Add status indicator widget as a child to HUD widget */
	UFUNCTION(BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Status Indicator Widget To Canvas"))
	void BP_AddStatusIndicatorWidgetToCanvas(UStatusIndicatorWidget* NewWidget);

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SkillBarWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SkillBarWidgetPosition;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAnchors SkillBarWidgetAnchor;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SkillTreeWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SkillTreeWidgetPosition;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D InventoryWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D InventoryWidgetPosition;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D StatusIndicatorWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D StatusIndicatorWidgetPosition;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D DialogueWidgetSize;

	UPROPERTY(Category = WidgetParams, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D DialogueWidgetPosition;

};

FORCEINLINE UStatusIndicatorWidget* UHUDWidget::GetStatusIndicatorWidget() const
{
	return StatusIndicatorWidget;
}

FORCEINLINE USkillBarWidget* UHUDWidget::GetSkillBarWidget() const
{
	return SkillBarWidget;
}

FORCEINLINE UInventoryWidget* UHUDWidget::GetInventoryWidget() const
{
	return InventoryWidget;
}

FORCEINLINE USkillTreeWidget* UHUDWidget::GetSkillTreeWidget() const
{
	return SkillTreeWidget;
}

inline UPlayerStatsWidget* UHUDWidget::GetPlayerStatsWidget() const
{
	return PlayerStatsWidget;
}

FORCEINLINE UCanvasPanel* UHUDWidget::GetMainCanvas() const
{
	return MainCanvas;
}

inline void UHUDWidget::AddSkillBarWidget(USkillBarWidget* NewWidget)
{
	if (SkillBarWidget == NewWidget)
	{
		return;
	}

	if (SkillBarWidget)
	{
		MainCanvas->RemoveChild(SkillBarWidget);
		SkillBarWidget = nullptr;
	}

	if (NewWidget)
	{
		UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
		CPSlot->SetSize(SkillBarWidgetSize);
		CPSlot->SetPosition(SkillBarWidgetPosition);
		CPSlot->SetAnchors(SkillBarWidgetAnchor);

		SkillBarWidget = NewWidget;
	}
}

inline void UHUDWidget::AddSkillTreeWidget(USkillTreeWidget* NewWidget)
{
	if (SkillTreeWidget == NewWidget)
	{
		return;
	}

	if (SkillTreeWidget)
	{
		MainCanvas->RemoveChild(SkillTreeWidget);
		SkillTreeWidget = nullptr;
	}

	if (NewWidget)
	{
		UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
		CPSlot->SetSize(SkillTreeWidgetSize);
		CPSlot->SetPosition(SkillTreeWidgetPosition);

		SkillTreeWidget = NewWidget;
	}
}

inline void UHUDWidget::AddInventoryWidget(UInventoryWidget* NewWidget)
{
	if (InventoryWidget == NewWidget)
	{
		return;
	}

	if (InventoryWidget)
	{
		MainCanvas->RemoveChild(InventoryWidget);
		InventoryWidget = nullptr;
	}

	if (NewWidget)
	{
		UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
		CPSlot->SetSize(InventoryWidgetSize);
		CPSlot->SetPosition(InventoryWidgetPosition);

		InventoryWidget = NewWidget;
	}
}

inline void UHUDWidget::AddStatusIndicatorWidget(UStatusIndicatorWidget* NewWidget)
{
	if (StatusIndicatorWidget == NewWidget)
	{
		return;
	}

	if (StatusIndicatorWidget)
	{
		MainCanvas->RemoveChild(StatusIndicatorWidget);
		StatusIndicatorWidget = nullptr;
	}

	if (NewWidget)
	{
		UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
		CPSlot->SetSize(StatusIndicatorWidgetSize);
		CPSlot->SetPosition(StatusIndicatorWidgetPosition);

		StatusIndicatorWidget = NewWidget;
	}
}

inline void UHUDWidget::AddDialogueWidget(UDialogueWindowWidget* NewWidget)
{
	// @todo - improve?

	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(DialogueWidgetSize);
	CPSlot->SetPosition(DialogueWidgetPosition);
}
