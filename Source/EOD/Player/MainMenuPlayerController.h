// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

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

	// UPROPERTY(Transient, BlueprintReadOnly, Category = "Main Menu UI")
	// UUserWidget* OptionsWidget;
	//~ End UI


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
