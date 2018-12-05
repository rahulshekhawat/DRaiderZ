// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EOD/UI/EODItemContainer.h"
#include "EOD/UI/SkillBarWidget.h"
#include "EOD/UI/SkillTreeWidget.h"
#include "EOD/UI/InventoryWidget.h"
#include "EOD/UI/StatusIndicatorWidget.h"

#include "UObject/ObjectMacros.h"
#include "Blueprint/UserWidget.h"
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

	UHUDWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;	

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/** Widget containing health, mana, and stamina bars */
	UPROPERTY(Transient, BlueprintReadWrite)
	UStatusIndicatorWidget* StatusIndicatorWidget;

	/** Widget containing skills that can be used by player */
	UPROPERTY(Transient, BlueprintReadWrite)
	USkillBarWidget* SkillBarWidget;

	/** Widget containing player inventory items */
	UPROPERTY(Transient, BlueprintReadWrite)
	UInventoryWidget* InventoryWidget;

	/** Widget containing skill trees of all vocations */
	UPROPERTY(Transient, BlueprintReadWrite)
	USkillTreeWidget* SkillTreeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	/** Save current HUD layout to the current save slot */
	void SaveHUDLayout();

	/** Add skill bar widget as a child to HUD widget */
	inline void AddSkillBarWidget(USkillBarWidget* NewWidget);

	/** Add skill tree widget as a child to HUD widget */
	inline void AddSkillTreeWidget(USkillTreeWidget* NewWidget);

	/** Add inventory widget as a child to HUD widget */
	inline void AddInventoryWidget(UInventoryWidget* NewWidget);

	/** Add dialogue widget as a child to HUD widget */
	inline void AddDialogueWidget(UUserWidget* NewWidget);

	/** Add status indicator widget as a child to HUD widget */
	inline void AddStatusIndicatorWidget(UStatusIndicatorWidget* NewWidget);

	/** Add skill bar widget as a child to HUD widget */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Skill Bar Widget To Canvas"))
	void BP_AddSkillBarWidgetToCanvas(USkillBarWidget* NewWidget);

	/** Add skill tree widget as a child to HUD widget */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Skill Tree Widget To Canvas"))
	void BP_AddSkillTreeWidgetToCanvas(USkillBarWidget* NewWidget);

	/** Add inventory widget as a child to HUD widget */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Inventory Widget To Canvas"))
	void BP_AddInventoryWidgetToCanvas(USkillBarWidget* NewWidget);

	/** Add status indicator widget as a child to HUD widget */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "HUD Widget", meta = (DisplayName = "Add Status Indicator Widget To Canvas"))
	void BP_AddStatusIndicatorWidgetToCanvas(USkillBarWidget* NewWidget);

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

inline void UHUDWidget::AddSkillBarWidget(USkillBarWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(SkillBarWidgetSize);
	CPSlot->SetPosition(SkillBarWidgetPosition);
	CPSlot->SetAnchors(SkillBarWidgetAnchor);

	SkillBarWidget = NewWidget;
}

inline void UHUDWidget::AddSkillTreeWidget(USkillTreeWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(SkillTreeWidgetSize);
	CPSlot->SetPosition(SkillTreeWidgetPosition);

	SkillTreeWidget = NewWidget;
}

inline void UHUDWidget::AddInventoryWidget(UInventoryWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(InventoryWidgetSize);
	CPSlot->SetPosition(InventoryWidgetPosition);

	InventoryWidget = NewWidget;
}

inline void UHUDWidget::AddDialogueWidget(UUserWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(DialogueWidgetSize);
	CPSlot->SetPosition(DialogueWidgetPosition);
}

inline void UHUDWidget::AddStatusIndicatorWidget(UStatusIndicatorWidget * NewWidget)
{
	UCanvasPanelSlot* CPSlot = MainCanvas->AddChildToCanvas(NewWidget);
	CPSlot->SetSize(StatusIndicatorWidgetSize);
	CPSlot->SetPosition(StatusIndicatorWidgetPosition);

	StatusIndicatorWidget = NewWidget;
}
