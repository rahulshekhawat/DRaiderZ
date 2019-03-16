// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

class UHUDWidget;
class UDynamicHUDWidget;
class UPauseMenuWidget;
class USkillTreeComponent;
class APlayerSkillTreeManager;
class USkillTreeWidget;
class USkillsComponent;
class UInventoryComponent;
class UStatsComponentBase;
class UDialogueWindowWidget;
class UPlayerStatsComponent;

/**
 * EmberPlayerController is the base (and final c++) class for EOD's player controller
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
	//  
	// --------------------------------------

	FORCEINLINE AEODCharacterBase* GetEODCharacter() const { return EODCharacter; }

	void LoadPlayerState();

private:

	UPROPERTY()
	AEODCharacterBase* EODCharacter;

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
	//  Player Skill Tree
	// --------------------------------------

	FORCEINLINE APlayerSkillTreeManager* GetLocalSkillTreeManager() const { return LocalSkillTreeManager; }

	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

private:

	UPROPERTY(Transient)
	APlayerSkillTreeManager* LocalSkillTreeManager;

	UPROPERTY(Transient)
	USkillTreeWidget* SkillTreeWidget;

public:

	// --------------------------------------
	//  User Interface
	// --------------------------------------

	FORCEINLINE UDynamicHUDWidget* GetHUDWidget() const { return HUDWidget; }

	FORCEINLINE UDialogueWindowWidget* GetDialogueWidget() const { return DialogueWidget; }

	FORCEINLINE UPauseMenuWidget* GetPauseMenuWidget() const { return PauseMenuWidget; }

private:
	/** Player's head-up display widget */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player UI", meta = (AllowPrivateAccess = "true"))
	UDynamicHUDWidget* HUDWidget;

	/** The widget class to use for player's head-up display */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UDynamicHUDWidget> HUDWidgetClass;

	/** Dialogue widget used to display NPC dialogues */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Player UI", meta = (AllowPrivateAccess = "true"))
	UDialogueWindowWidget* DialogueWidget;

	/** The widget class used for dialogue widget */
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UDialogueWindowWidget> DialogueWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player UI", meta = (AllowPrivateAccess = "true"))
	UPauseMenuWidget* PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	void CreateHUDWidget();

	void InitStatusIndicatorWidget();

	void InitInventoryWidget();

	void InitSkillTreeWidget();

	void InitSkillBarWidget();

public:

	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	FORCEINLINE void SwitchToUIInput();

	FORCEINLINE void SwitchToGameInput();

	void TogglePlayerStatsUI();

	void TogglePlayerHUD();

	void TogglePlayerSkillTreeUI();

	void TogglePlayerInventoryUI();

private:
	void OnPressedForward();

	void OnReleasedForward();

	void OnPressedBackward();

	void OnReleasedBackward();

	bool bAutoMoveEnabled;

	FORCEINLINE bool IsAutoMoveEnabled() const;

	FORCEINLINE void SetAutoMoveEnabled(bool bValue);

	FORCEINLINE void EnableAutoMove();

	FORCEINLINE void DisableAutoMove();

	/** Move controlled pawn forward/backward */
	void MovePawnForward(const float Value);

	/** Move controlled pawn left/right */
	void MovePawnRight(const float Value);

	void MakePawnJump();

	void ZoomInCamera();

	void ZoomOutCamera();

	void OnPressingNormalAttackKey();

	void OnReleasingNormalAttackKey();

	void AttemptDodge();

	void TriggerInteraction();

	void ToggleAutoMove();

	void ToggleMouseCursor();

	/** Put or remove weapon inside sheath */
	void ToggleSheathe();

	void OnPressingGuardKey();

	void OnReleasingGuardKey();

	void OnPressingEscapeKey();

	void OnPressingSkillKey(const int32 SkillKeyIndex);

	void OnReleasingSkillKey(const int32 SkillKeyIndex);

	/** Called when player presses a skill key */
	template<uint32 SkillKeyIndex>
	inline void PressedSkillKey();

	/** Called when player releases a skill key */
	template<uint32 SkillKeyIndex>
	inline void ReleasedSkillKey();

public:

	// --------------------------------------
	//  Save/Load System
	// --------------------------------------

	/** Saves current player state */
	UFUNCTION(BlueprintCallable, Category = "Save/Load System")
	void SavePlayerState();

private:

	// --------------------------------------
	//  Constants
	// --------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = "Player Constants")
	int32 DodgeStaminaCost;


private:

	// --------------------------------------
	//  Network : RPCs and Rep Notifies
	// --------------------------------------

	UFUNCTION(Client, Reliable)
	void Client_SetupLocalPlayerOnUnpossess(APawn* InPawn);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnSuccessfulDodge();

	friend class AEODCharacterBase;

};

FORCEINLINE void AEODPlayerController::SwitchToUIInput()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetHideCursorDuringCapture(false);
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

FORCEINLINE bool AEODPlayerController::IsAutoMoveEnabled() const
{
	return bAutoMoveEnabled;
}

FORCEINLINE void AEODPlayerController::SetAutoMoveEnabled(bool bValue)
{
	bAutoMoveEnabled = bValue;
}

FORCEINLINE void AEODPlayerController::EnableAutoMove()
{
	SetAutoMoveEnabled(true);
	if (EODCharacter)
	{
		EODCharacter->SetUseControllerRotationYaw(true);
	}
}

FORCEINLINE void AEODPlayerController::DisableAutoMove()
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
