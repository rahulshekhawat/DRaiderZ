// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UMenuWidgetBase;
class UPlayerSaveGame;

/**
 * 
 */
UCLASS()
class EOD_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AMainMenuPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	// --------------------------------------
	//  User Interface
	// --------------------------------------

	void CreateMenuWidgets();

	/** Replaces current widget with title screen widget. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SwitchToTitleScreenWidget();
	virtual void SwitchToTitleScreenWidget_Implementation();


	/**
	 * Replaces current widget with main menu widget.
	 * @param PlayerSaveGame It is used to initialize the main menu widget state (e.g., to determine whether 'CONTINUE' button should be enabled in main menu)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SwitchToMainMenuWidget(UPlayerSaveGame* PlayerSaveGame = nullptr);
	virtual void SwitchToMainMenuWidget_Implementation(UPlayerSaveGame* PlayerSaveGame = nullptr);

	/** Replaces current widget with new profile creation widget. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SwitchToNewProfileCreationWidget();
	virtual void SwitchToNewProfileCreationWidget_Implementation();

	/** Replaces current widget with multiplayer options widget. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SwitchToMultiplayerWidget();
	virtual void SwitchToMultiplayerWidget_Implementation();

	/** Replaces current widget with settings widget. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SwitchToSettingsWidget();
	virtual void SwitchToSettingsWidget_Implementation();

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UMenuWidgetBase* ActiveWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UMenuWidgetBase> TitleScreenWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UMenuWidgetBase* TitleScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UMenuWidgetBase> MainMenuWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UMenuWidgetBase* MainMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UMenuWidgetBase> NewProfileCreationWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UMenuWidgetBase* NewProfileCreationWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UMenuWidgetBase> MultiplayerWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UMenuWidgetBase* MultiplayerWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UMenuWidgetBase> SettingsWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UMenuWidgetBase* SettingsWidget;

	inline void SwitchToUIInput();

	void CreatePlayerMenu();

	inline void CreateTitleScreenWidget();
	inline void CreateMainMenuWidget();
	inline void CreateSettingsWidget();
	inline void CreateMultiplayerWidget();
	inline void CreateNewProfileCreationWidget();

	// --------------------------------------
	//  Utility
	// --------------------------------------

	/** Begin a new campaign */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void StartNewCampaign();

	/** This method can only be called from 'CreateNewProfileWidget' */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void CreateAndLoadNewProfile(const FString& NewProfileName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Utility")
	void HandleTitleScreenAnyKeyEvent(const FKey& Key);
	void HandleTitleScreenAnyKeyEvent_Implementation(const FKey& Key);

};

inline void AMainMenuPlayerController::SwitchToUIInput()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetHideCursorDuringCapture(false);
	GameAndUIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(GameAndUIInputMode);
}

inline void AMainMenuPlayerController::CreateTitleScreenWidget()
{
	if (!IsValid(TitleScreenWidget) && TitleScreenWidgetClass.Get())
	{
		TitleScreenWidget = CreateWidget<UMenuWidgetBase>(this, TitleScreenWidgetClass);
	}
}

inline void AMainMenuPlayerController::CreateMainMenuWidget()
{
	if (!IsValid(MainMenuWidget) && MainMenuWidgetClass.Get())
	{
		MainMenuWidget = CreateWidget<UMenuWidgetBase>(this, MainMenuWidgetClass);
	}
}

inline void AMainMenuPlayerController::CreateSettingsWidget()
{
	if (!IsValid(SettingsWidget) && SettingsWidgetClass.Get())
	{
		SettingsWidget = CreateWidget<UMenuWidgetBase>(this, SettingsWidgetClass);
	}
}

inline void AMainMenuPlayerController::CreateMultiplayerWidget()
{
	if (!IsValid(MultiplayerWidget) && MultiplayerWidgetClass.Get())
	{
		MultiplayerWidget = CreateWidget<UMenuWidgetBase>(this, MultiplayerWidgetClass);
	}
}

inline void AMainMenuPlayerController::CreateNewProfileCreationWidget()
{
	if (!IsValid(NewProfileCreationWidget) && NewProfileCreationWidgetClass.Get())
	{
		NewProfileCreationWidget = CreateWidget<UMenuWidgetBase>(this, NewProfileCreationWidgetClass);
	}
}
