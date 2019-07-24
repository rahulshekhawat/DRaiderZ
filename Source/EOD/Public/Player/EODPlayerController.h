// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"
#include "CombatLibrary.h"
#include "EODCharacterBase.h"
#include "HUDWidget.h"
#include "LootableInterface.h"

#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

class UGameplaySkillBase;
class UActiveSkillBase;
// class UHUDWidget;
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

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	AEODPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetPawn(APawn* InPawn) override;


	///////////////////////////////////////////////////////////////////////////
	//
public:

	/** Returns the gender of the pawn that the player selected during player creation */
	FORCEINLINE ECharacterGender GetGender() const { return Gender; }

	FORCEINLINE AEODCharacterBase* GetEODCharacter() const { return EODCharacter; }

	UFUNCTION(BlueprintCallable, Category = Utility)
	void AddEXP(int32 Value);

private:
	
	UPROPERTY(EditDefaultsOnly, Category = Constants)
	int32 DodgeStaminaCost;

	UPROPERTY(Transient)
	AEODCharacterBase* EODCharacter;

	/** The gender of the pawn that the player selected during player creation */
	UPROPERTY(Transient)
	ECharacterGender Gender;
	
	UPROPERTY(Transient)
	int32 LeveupEXP;
	
	void SetGender(ECharacterGender NewGender);

	void SetLeveupEXP(int32 EXP);


	///////////////////////////////////////////////////////////////////////////
	//  Components
public:

	inline UInventoryComponent* GetInventoryComponent() const;

	inline USkillTreeComponent* GetSkillTreeComponent() const;

	inline UPlayerStatsComponent* GetStatsComponent() const;

	static const FName InventoryComponentName;
	
	static const FName SkillTreeComponentName;

	static const FName StatsComponentName;

protected:

	/** Player inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* InventoryComponent;

	/** Player skill tree */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USkillTreeComponent* SkillTreeComponent;

	/** Player Stats */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UPlayerStatsComponent* StatsComponent;


	///////////////////////////////////////////////////////////////////////////
	//  UI
public:

	inline UHUDWidget* GetHUDWidget() const;

	inline UDialogueWindowWidget* GetDialogueWidget() const;

	inline UInGameMenuWidget* GetPauseMenuWidget() const;

	inline USkillBarWidget* GetSkillBarWidget() const;

	inline USkillTreeWidget* GetSkillTreeWidget() const;

	inline UInventoryWidget* GetInventoryWidget() const;

	inline UPlayerStatsWidget* GetPlayerStatsWidget() const;

	inline UStatusIndicatorWidget* GetStatusIndicatorWidget() const;

	UFUNCTION(BlueprintCallable, Category = UI)
	UInteractivePopupWidget* GetActivePopupWidget() const;

	UFUNCTION(BlueprintCallable, Category = UI)
	void RegisterPopupWidget(UObject* RegisteringObj, const FString& InKeyText, const FString& InDetailText, UTexture* InIcon = nullptr);

	UFUNCTION(BlueprintCallable, Category = UI)
	void UnregisterPopupWidget(UObject* RegisteringObj);

	UFUNCTION(BlueprintCallable, Category = UI)
	void UnregisterActivePopupWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = UI)
	bool CreateLootWidget(const TArray<FGeneratedLootInfo>& LootInfoArray, UObject* LootSource);
	virtual bool CreateLootWidget_Implementation(const TArray<FGeneratedLootInfo>& LootInfoArray, UObject* LootSource);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = UI)
	void RemoveLootWidget(UObject* LootSource);
	virtual void RemoveLootWidget_Implementation(UObject* LootSource);

protected:
	
	/** The widget class to use for player's head-up display */
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UHUDWidget> HUDWidgetClass;
	
	/** The widget class used for dialogue widget */
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UDialogueWindowWidget> DialogueWidgetClass;
	
	/** The widget class used for in-game widget */
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInGameMenuWidget> PauseMenuWidgetClass;

	/** Player's head-up display widget */
	UPROPERTY(Transient, BlueprintReadOnly, Category = UI)
	UHUDWidget* HUDWidget;

	/** Dialogue widget used to display NPC dialogues */
	UPROPERTY(Transient, BlueprintReadOnly, Category = UI)
	UDialogueWindowWidget* DialogueWidget;

	/** In-game menu widget that can be brought up when player presses escape key */
	UPROPERTY(Transient, BlueprintReadOnly, Category = UI)
	UInGameMenuWidget* InGameMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* UIUpSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* UIDownSound;


	///////////////////////////////////////////////////////////////////////////
	//  UI Initialization
public:

	void CreateHUDWidget();
	void InitWidgets();
	void BindWidgetDelegates();
	void UnbindWidgetDelegates();

private:

	void InitHUDWidget();
	void InitStatusIndicatorWidget();
	void InitSkillTreeWidget();
	void InitSkillBarWidget();
	void InitInventoryWidget();
	void InitPlayerStatsWidget();

	void BindHUDDelegates();
	void BindStatusIndicatorDelegates();
	void BindSkillTreeDelegates();
	void BindSkillBarDelegates();
	void BindInventoryDelegates();
	void BindPlayerStatsDelegates();

	void UnbindHUDDelegates();
	void UnbindStatusIndicatorDelegates();
	void UnbindSkillTreeDelegates();
	void UnbindSkillBarDelegates();
	void UnbindInventoryDelegates();
	void UnbindPlayerStatsDelegates();


	///////////////////////////////////////////////////////////////////////////
	//  Input Handling
public:

	/** Returns true if auto move is enabled */
	FORCEINLINE bool IsAutoMoveEnabled() const { return bAutoMoveEnabled; }

	/** Switch to UI input mode */
	inline void SwitchToUIInput();

	/** Switch to game input mode */
	inline void SwitchToGameInput();

	/** Toggle display of HUDWidget */
	void TogglePlayerHUD();

	/** Toggle display of PlayerStatsWidget */
	void TogglePlayerStatsUI();

	/** Toggle display of SkillTreeWidget */
	void TogglePlayerSkillTreeUI();

	/** Toggle display of InventoryWidget */
	void TogglePlayerInventoryUI();

	void TogglePlayerJournalUI();

	void TogglePlayerWeapon();

	FName GetKeyNameForActionName(FName InActionName);

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

	void OnPressedLeft();
	void OnReleasedLeft();
	void OnPressedRight();
	void OnReleasedRight();

	/** Set whether possessed pawn should move automatically or not */
	inline void SetAutoMoveEnabled(bool bValue);

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


	///////////////////////////////////////////////////////////////////////////
	//  Save and Load System
public:

	UFUNCTION(BlueprintCallable, Category = "Save/Load System")
	void SavePlayerState();

	UFUNCTION(BlueprintCallable, Category = "Save/Load System")
	void LoadPlayerState();


	///////////////////////////////////////////////////////////////////////////
	//  Network
protected:

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

inline UInventoryComponent* AEODPlayerController::GetInventoryComponent() const
{
	return InventoryComponent;
}

inline USkillTreeComponent* AEODPlayerController::GetSkillTreeComponent() const
{
	return SkillTreeComponent;
}

inline UPlayerStatsComponent* AEODPlayerController::GetStatsComponent() const
{
	return StatsComponent;
}

inline UHUDWidget* AEODPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

inline UDialogueWindowWidget* AEODPlayerController::GetDialogueWidget() const
{
	return DialogueWidget;
}

inline UInGameMenuWidget* AEODPlayerController::GetPauseMenuWidget() const
{
	return InGameMenuWidget;
}

inline USkillBarWidget* AEODPlayerController::GetSkillBarWidget() const
{
	return HUDWidget ? HUDWidget->GetSkillBarWidget() : nullptr;
}

inline USkillTreeWidget* AEODPlayerController::GetSkillTreeWidget() const
{
	return HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;
}

inline UInventoryWidget* AEODPlayerController::GetInventoryWidget() const
{
	return HUDWidget ? HUDWidget->GetInventoryWidget() : nullptr;
}

inline UPlayerStatsWidget* AEODPlayerController::GetPlayerStatsWidget() const
{
	return HUDWidget ? HUDWidget->GetPlayerStatsWidget() : nullptr;
}

inline UStatusIndicatorWidget* AEODPlayerController::GetStatusIndicatorWidget() const
{
	return HUDWidget ? HUDWidget->GetStatusIndicatorWidget() : nullptr;
}

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

inline void AEODPlayerController::SetAutoMoveEnabled(bool bValue)
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
