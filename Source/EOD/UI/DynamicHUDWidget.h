// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicHUDWidget.generated.h"

class UTextBlock;
class UCanvasPanel;
class USkillBarWidget;
class UInventoryWidget;
class UPlayerStatsWidget;
class UStatusIndicatorWidget;
class UDialogueWindowWidget;
class UDynamicSkillTreeWidget;

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
	UPROPERTY(Transient, BlueprintReadOnly)
	USkillBarWidget* SkillBarWidget;

public:

	FORCEINLINE UCanvasPanel* GetMainCanvas() const { return MainCanvas; }

	FORCEINLINE UStatusIndicatorWidget* GetStatusIndicatorWidget() const { return StatusIndicatorWidget; }

	FORCEINLINE USkillBarWidget* GetSkillBarWidget() const { return SkillBarWidget; }

	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	FORCEINLINE UDynamicSkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

	FORCEINLINE UPlayerStatsWidget* GetPlayerStatsWidget() const { return PlayerStatsWidget; }

	/** Add skill bar widget as a child to HUD widget */
	void AddSkillBarWidget(USkillBarWidget* NewWidget);

	/** Add dialogue widget as a child to HUD widget */
	void AddDialogueWidget(UDialogueWindowWidget* NewWidget);

private:

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

};
