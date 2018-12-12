// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

class UHUDWidget;
class UInventoryComponent;
class UStatsComponentBase;

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


	////////////////////////////////////////////////////////////////////////////////
	// COMPONENTS
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE UStatsComponentBase* GetStatsComponent() const;

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const;

private:
	//~ Inventory component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	//~ StatsComp contains and manages the stats of player
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponentBase* StatsComponent;


	////////////////////////////////////////////////////////////////////////////////
	// UI
	////////////////////////////////////////////////////////////////////////////////
private:
	/** Player HUD class reference */
	UPROPERTY(Transient)
	UHUDWidget* HUDWidget;

	/** The blueprint widget class to use for player HUD */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

public:
	FORCEINLINE UHUDWidget* GetHUDWidget() const;


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
