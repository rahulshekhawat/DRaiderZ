// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

class UHUDWidget;
// class USkillBarWidget;
// class USkillTreeWidget;
// class UPlayerStatsWidget;
class USkillsComponent;
class UInventoryComponent;
class UStatsComponentBase;
class UDialogueWindowWidget;

/**
 * EmberPlayerController is the base (and final c++) class for EOD's player controller
 * @note All blueprint classes for player controller must inherit from EmberPlayerController
 */
UCLASS()
class EOD_API AEODPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AEODPlayerController(const FObjectInitializer& ObjectInitializer);

	/** Binds functionality for mouse axis input */
	virtual void SetupInputComponent() override;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	////////////////////////////////////////////////////////////////////////////////
	// COMPONENTS
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE UStatsComponentBase* GetStatsComponent() const;

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const;

	FORCEINLINE USkillsComponent* GetSkillsComponent() const;

private:
	//~ Inventory component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	//~ StatsComp contains and manages the stats of player
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponentBase* StatsComponent;

	//~ Skills component manages and displays the skills of possessed pawn (player or not)
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillsComponent* SkillsComponent;


	////////////////////////////////////////////////////////////////////////////////
	// UI
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE UHUDWidget* GetHUDWidget() const;

private:
	/** Player's head-up display widget */
	UPROPERTY(Transient)
	UHUDWidget* HUDWidget;

	/** The widget class to use for player's head-up display */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	/** Dialogue widget used to display NPC dialogues */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Player UI", meta = (AllowPrivateAccess = "true"))
	UDialogueWindowWidget* DialogueWidget;

	/** The widget class used for dialogue widget */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UDialogueWindowWidget> DialogueWidgetClass;

	void CreateHUDWidget();

	void InitStatusIndicatorWidget();

	/** Initialized inventory widget and also sets up proper delegates */
	void InitInventoryWidget();

	void InitSkillTreeWidget();

	void InitSkillBarWidget();


	////////////////////////////////////////////////////////////////////////////////
	// INPUT HANDLING
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE void SwitchToUIInput();

	FORCEINLINE void SwitchToGameInput();

	void TogglePlayerStatsUI();

	void TogglePlayerHUD();

	void TogglePlayerSkillTreeUI();

	void TogglePlayerInventoryUI();

private:
	/** Move controlled pawn forward/backward */
	void MovePawnForward(const float Value);

	/** Move controlled pawn left/right */
	void MovePawnRight(const float Value);

	void MakePawnJump();

	void ZoomInCamera();

	void ZoomOutCamera();

	void TriggerInteraction();

	void ToggleAutoRun();

	void ToggleMouseCursor();

	void OnPressingEscapeKey();

	void OnPressingSkillKey(const int32 SkillKeyIndex);

	void OnReleasingSkillKey(const int32 SkillKeyIndex);

	/** Called when player presses a skill key */
	template<uint32 SkillKeyIndex>
	inline void PressedSkillKey();

	/** Called when player releases a skill key */
	template<uint32 SkillKeyIndex>
	inline void ReleasedSkillKey();

	
	////////////////////////////////////////////////////////////////////////////////
	// CONSTANTS
	////////////////////////////////////////////////////////////////////////////////
private:
	const int CameraZoomRate = 15;

	const int CameraArmMinimumLength = 50;

	const int CameraArmMaximumLength = 500;

};

FORCEINLINE UStatsComponentBase* AEODPlayerController::GetStatsComponent() const
{
	return StatsComponent;
}

FORCEINLINE UInventoryComponent* AEODPlayerController::GetInventoryComponent() const
{
	return InventoryComponent;
}

FORCEINLINE USkillsComponent* AEODPlayerController::GetSkillsComponent() const
{
	return SkillsComponent;
}

FORCEINLINE void AEODPlayerController::SwitchToUIInput()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(GameAndUIInputMode);
}

FORCEINLINE void AEODPlayerController::SwitchToGameInput()
{
	bShowMouseCursor = false;
	FInputModeGameOnly GameOnlyInputMode;
	GameOnlyInputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(GameOnlyInputMode);
}

FORCEINLINE UHUDWidget* AEODPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

template<uint32 SkillKeyIndex>
inline void AEODPlayerController::PressedSkillKey()
{
	OnPressingSkillKey(SkillKeyIndex);
}

template<uint32 SkillKeyIndex>
inline void AEODPlayerController::ReleasedSkillKey()
{
	OnReleasingSkillKey(SkillKeyIndex);
}
