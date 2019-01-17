// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"


class UPlayerSaveGame;

/**
 * 
 */
UCLASS()
class EOD_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//~ Begin UI
public:
	UFUNCTION(BlueprintCallable, Category = "Main Menu UI")
	void SwitchToTitleScreenWidget();

	/**
	 * Replaces current widget with main menu widget.
	 * @param PlayerSaveGame It is used to initialize the main menu widget state (e.g., to determine whether 'CONTINUE' button should be enabled in main menu)
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu UI")
	void SwitchToMainMenuWidget(UPlayerSaveGame* PlayerSaveGame = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Main Menu UI")
	void SwitchToCreateNewProfileWidget();

	UFUNCTION(BlueprintCallable, Category = "Main Menu UI")
	void SwitchToMultiplayerWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Main Menu UI")
	void StartNewCampaign();

	void StartNewCampaign_Implementation();

	// UFUNCTION(BlueprintCallable, Category = "Main Menu UI")


protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UUserWidget* ActiveWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UUserWidget> TitleScreenWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UUserWidget* TitleScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UUserWidget* MainMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UUserWidget> CreateNewProfileWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UUserWidget* CreateNewProfileWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu UI")
	TSubclassOf<UUserWidget> MultiplayerWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	UUserWidget* MultiplayerWidget;
	//~ End UI


	/** This method can only be called from 'CreateNewProfileWidget' */
	UFUNCTION(BlueprintCallable, Category = "Main Menu UI")
	void CreateAndLoadNewProfile(const FString& NewProfileName);

	UFUNCTION(BlueprintCallable, Category = "Main Menu UI")
	void HandleTitleScreenAnyKeyEvent(const FKey& Key);
	//~ 
private:

	inline void SwitchToUIInput();


};

inline void AMainMenuPlayerController::SwitchToUIInput()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUIInputMode;
	GameAndUIInputMode.SetHideCursorDuringCapture(false);
	GameAndUIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(GameAndUIInputMode);
}
