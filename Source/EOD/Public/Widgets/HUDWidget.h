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
protected:
	
	/** The main canvas that contains all the child widgets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	/** Widget containing health, mana, and stamina bars */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UStatusIndicatorWidget* StatusIndicatorWidget;

	/** Widget containing player stats info */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlayerStatsWidget* PlayerStatsWidget;

	/** Widget containing player inventory items */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UInventoryWidget* InventoryWidget;

	/** Widget containing skill trees of all vocations */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillTreeWidget* SkillTreeWidget;
	
	/** Widget containing skills that the player can actively use */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USkillBarWidget* SkillBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UInteractivePopupWidget* InteractivePopup;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerLevel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerName;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetPlayerLevel(int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetPlayerName(FString Name);


};
