// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "GameFramework/PlayerController.h"
#include "EODPlayerController.generated.h"

class UHUDWidget;
class UPauseMenuWidget;
class USkillTreeComponent;
// class UPlayerStatsWidget;
// class AEODCharacterBase;
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;

	virtual void UnPossess() override;

	virtual void SetPawn(APawn* InPawn) override;

	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY()
	AEODCharacterBase* EODCharacter;

public:
	FORCEINLINE AEODCharacterBase* GetEODCharacter() const { return EODCharacter; }

	void LoadPlayerState();

	////////////////////////////////////////////////////////////////////////////////
	// COMPONENTS
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const;

	FORCEINLINE USkillTreeComponent* GetSkillTreeComponent() const { return SkillTreeComponent; }

	static FName InventoryComponentName;
	
	static FName SkillTreeComponentName;

private:
	//~ Inventory component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillTreeComponent* SkillTreeComponent;


	////////////////////////////////////////////////////////////////////////////////
	// UI
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE UHUDWidget* GetHUDWidget() const;

private:
	/** Player's head-up display widget */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player UI", meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Player UI", meta = (AllowPrivateAccess = "true"))
	UPauseMenuWidget* PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

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
	void OnPressedForward();

	void OnReleasedForward();

	void OnPressedBackward();

	void OnReleasedBackward();

	// UPROPERTY(Replicated)
	bool bAutoMoveEnabled;

	FORCEINLINE bool IsAutoMoveEnabled() const;

	FORCEINLINE void SetAutoMoveEnabled(bool bValue);

	FORCEINLINE void EnableAutoMove();

	FORCEINLINE void DisableAutoMove();

	FORCEINLINE bool IsBlockKeyPressed() const
	{
		return IsValid(EODCharacter) ? EODCharacter->IsBlockKeyPressed() : false;
	}

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


	////////////////////////////////////////////////////////////////////////////////
	// SAVE/LOAD SYSTEM
	////////////////////////////////////////////////////////////////////////////////
public:
	/** Saves current player state */
	UFUNCTION(BlueprintCallable, Category = "Save/Load System")
	void SavePlayerState();


	////////////////////////////////////////////////////////////////////////////////
	// CONSTANTS
	////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "Player Constants")
	int32 DodgeStaminaCost;


	////////////////////////////////////////////////////////////////////////////////
	// NETWORK
	////////////////////////////////////////////////////////////////////////////////
private:
	UFUNCTION(Client, Reliable)
	void Client_SetupLocalPlayerOnUnpossess(APawn* InPawn);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnSuccessfulDodge();

};

FORCEINLINE UInventoryComponent* AEODPlayerController::GetInventoryComponent() const
{
	return InventoryComponent;
}

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
