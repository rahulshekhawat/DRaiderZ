// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODCharacterBase.h"
#include "CombatLibrary.h"

#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

class UGameplaySkillBase;
class UActiveSkillBase;
class UHUDWidget;
class UDynamicHUDWidget;
class UDynamicSkillBarWidget;
class UInGameMenuWidget;
class USkillTreeComponent;
class APlayerSkillTreeManager;
class USkillTreeWidget;
class UInventoryComponent;
class UStatsComponentBase;
class UDialogueWindowWidget;
class UPlayerStatsComponent;

/**
 * EODPlayerController is the base (and final c++) class for in-game player controller
 * @note All blueprint classes for player controller must inherit from EmberPlayerController
 */
UCLASS()
class EOD_API AEODPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AEODPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetPawn(APawn* InPawn) override;

	// --------------------------------------
	//  Pawn
	// --------------------------------------

	/** Returns the gender of the pawn that the player selected during player creation */
	FORCEINLINE ECharacterGender GetGender() const { return Gender; }

	FORCEINLINE AEODCharacterBase* GetEODCharacter() const { return EODCharacter; }

protected:

	void SetGender(ECharacterGender NewGender);

private:

	UPROPERTY(Transient)
	AEODCharacterBase* EODCharacter;

	/** The gender of the pawn that the player selected during player creation */
	UPROPERTY(Transient)
	ECharacterGender Gender;

public:

	// --------------------------------------
	//  Components
	// --------------------------------------

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	FORCEINLINE USkillTreeComponent* GetSkillTreeComponent() const { return SkillTreeComponent; }

	FORCEINLINE UPlayerStatsComponent* GetStatsComponent() const { return StatsComponent; }

	static const FName InventoryComponentName;
	
	static const FName SkillTreeComponentName;

	static const FName StatsComponentName;

private:

	/** Player inventory */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	/** Player skill tree */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillTreeComponent* SkillTreeComponent;

	/** Player Stats */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPlayerStatsComponent* StatsComponent;

public:

	// --------------------------------------
	//  User Interface
	// --------------------------------------

	FORCEINLINE UDynamicHUDWidget* GetHUDWidget() const { return HUDWidget; }

	FORCEINLINE UDialogueWindowWidget* GetDialogueWidget() const { return DialogueWidget; }

	FORCEINLINE UInGameMenuWidget* GetPauseMenuWidget() const { return InGameMenuWidget; }

	UDynamicSkillBarWidget* GetSkillBarWidget() const;

protected:

	/** Player's head-up display widget */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player UI")
	UDynamicHUDWidget* HUDWidget;

	/** The widget class to use for player's head-up display */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UDynamicHUDWidget> HUDWidgetClass;

	/** Dialogue widget used to display NPC dialogues */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Player UI")
	UDialogueWindowWidget* DialogueWidget;

	/** The widget class used for dialogue widget */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UDialogueWindowWidget> DialogueWidgetClass;

	/** In-game menu widget that can be brought up when player presses escape key */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player UI")
	UInGameMenuWidget* InGameMenuWidget;

	/** The widget class used for in-game widget */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UInGameMenuWidget> PauseMenuWidgetClass;

private:

	void InitializeWidgets();

	void InitializeHUDWidget();

	void InitStatusIndicatorWidget();

	void InitInventoryWidget();

	void InitSkillTreeWidget();

	void InitSkillBarWidget();

	void InitPlayerStatsWidget();

public:

	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	/** Returns true if auto move is enabled */
	FORCEINLINE bool IsAutoMoveEnabled() const { return bAutoMoveEnabled; }

	/** Switch to UI input mode */
	inline void SwitchToUIInput();

	/** Switch to game input mode */
	inline void SwitchToGameInput();

	/** Toggle display of PlayerStatsWidget */
	void TogglePlayerStatsUI();

	/** Toggle display of HUDWidget */
	void TogglePlayerHUD();

	/** Toggle display of SkillTreeWidget */
	void TogglePlayerSkillTreeUI();

	/** Toggle display of InventoryWidget */
	void TogglePlayerInventoryUI();

	void TogglePlayerJournalUI();

	void TogglePlayerWeapon();

private:

	/** Determines whether automatic movement is enabled on possessed pawn */
	bool bAutoMoveEnabled;

	/** 
	 * Event called when player presses the forward key.
	 * This is used to set bForwardPressed boolean that is used to initiate forward lunging atack
	 */
	void OnPressedForward();

	/**
	 * Event called when player presses the forward key.
	 * This is used to set bForwardPressed boolean that is used to initiate forward lunging atack
	 */
	void OnReleasedForward();

	/**
	 * Event called when player presses the backward key.
	 * This is used to set bBackwardPressed boolean that is used to initiate backward lunging atack
	 */
	void OnPressedBackward();

	/**
	 * Event called when player presses the backward key.
	 * This is used to set bBackwardPressed boolean that is used to initiate backward lunging atack
	 */
	void OnReleasedBackward();

	/** Set whether possessed pawn should move automatically or not */
	FORCEINLINE void SetAutoMoveEnabled(bool bValue);

	/** Enable automatic movement */
	inline void EnableAutoMove();

	/** Disable automatic movement */
	inline void DisableAutoMove();

	/** Move controlled pawn forward/backward */
	void MovePawnForward(const float Value);

	/** Move controlled pawn left/right */
	void MovePawnRight(const float Value);

	/** Make the possessed pawn jump */
	void MakePawnJump();

	/** Zoom in pawn camera */
	void ZoomInCamera();

	/** Zoom out pawn camera */
	void ZoomOutCamera();

	/** Event called on pressing the normal attack key */
	void OnPressingNormalAttackKey();

	/** Event called on releasing the normal attack key */
	void OnReleasingNormalAttackKey();

	/** Attempt to make the possessed pawn dodge */
	void AttemptDodge();

	/** Attempt to interact with the nearest interactive actor */
	void TriggerInteraction();

	/** Enable or disable auto move */
	void ToggleAutoMove();

	/** Show or hide mouse cursor */
	void ToggleMouseCursor();

	/** Put or remove weapon inside sheath */
	void ToggleSheathe();

	/** Event called when player presses guard key */
	void OnPressingGuardKey();
	
	/** Event called when player releases guard key */
	void OnReleasingGuardKey();

	/** Event called when player presses escape key */
	void OnPressingEscapeKey();

	/** Called when player presses a skill key at given skill index */
	void OnPressingSkillKey(const int32 SkillKeyIndex);

	/** Called when player releases a skill key at given skill index */
	void OnReleasingSkillKey(const int32 SkillKeyIndex);

	/** Method bound to player input : called when a skill key is pressed */
	template<uint32 SkillKeyIndex>
	inline void PressedSkillKey();

	/** Method bound to player input : called when a skill key is released */
	template<uint32 SkillKeyIndex>
	inline void ReleasedSkillKey();

public:

	// --------------------------------------
	//  Save/Load System
	// --------------------------------------

	/** Saves current player state */
	UFUNCTION(BlueprintCallable, Category = "Save/Load System")
	void SavePlayerState();

	void LoadPlayerState();

private:

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = "Player Constants")
	int32 DodgeStaminaCost;

private:

	// --------------------------------------
	//  Network : RPCs and Rep Notifies
	// --------------------------------------

	/** Call this to set gender of player pawn on server */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetGender(ECharacterGender NewGender);
	virtual void Server_SetGender_Implementation(ECharacterGender NewGender);
	virtual bool Server_SetGender_Validate(ECharacterGender NewGender);

	/** Event called when the controlled pawn initiates dodge */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnInitiateDodge();
	virtual void Server_OnInitiateDodge_Implementation();
	virtual bool Server_OnInitiateDodge_Validate();

	UFUNCTION(Client, Reliable)
	void Client_SetupLocalPlayerOnUnpossess(APawn* InPawn);
	virtual void Client_SetupLocalPlayerOnUnpossess_Implementation(APawn* InPawn);

	friend class AEODCharacterBase;

};

inline void AEODPlayerController::SwitchToUIInput()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetHideCursorDuringCapture(false);
	GameAndUIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(GameAndUIInputMode);
}

inline void AEODPlayerController::SwitchToGameInput()
{
	bShowMouseCursor = false;
	FInputModeGameOnly GameOnlyInputMode;
	GameOnlyInputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(GameOnlyInputMode);
}

FORCEINLINE void AEODPlayerController::SetAutoMoveEnabled(bool bValue)
{
	bAutoMoveEnabled = bValue;
}

inline void AEODPlayerController::EnableAutoMove()
{
	SetAutoMoveEnabled(true);
	if (EODCharacter)
	{
		EODCharacter->SetUseControllerRotationYaw(true);
	}
}

inline void AEODPlayerController::DisableAutoMove()
{
	SetAutoMoveEnabled(false);
	if (EODCharacter)
	{
		EODCharacter->SetUseControllerRotationYaw(false);
	}
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
