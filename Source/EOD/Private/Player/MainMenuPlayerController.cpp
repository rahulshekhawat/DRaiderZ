// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MainMenuPlayerController.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"

#include "Kismet/GameplayStatics.h"

AMainMenuPlayerController::AMainMenuPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		CreateMenuWidgets();
		SwitchToTitleScreenWidget();
		SwitchToUIInput();
	}
}

void AMainMenuPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainMenuPlayerController::CreateMenuWidgets()
{
	if (IsLocalPlayerController())
	{
		CreateTitleScreenWidget();
		CreateMainMenuWidget();
		CreateSettingsWidget();
		CreateNewProfileCreationWidget();
	}
}

void AMainMenuPlayerController::SwitchToTitleScreenWidget_Implementation()
{
	if (ActiveWidget && ActiveWidget != TitleScreenWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	if (TitleScreenWidget)
	{
		ActiveWidget = TitleScreenWidget;
		ActiveWidget->AddMenuToScreen();
	}
}

void AMainMenuPlayerController::SwitchToMainMenuWidget_Implementation(UPlayerSaveGame* PlayerSaveGame)
{
	// If a proper player save game was not passed as an argument, we try to get the current player save game object from game instance
	if (!IsValid(PlayerSaveGame))
	{
		UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
		PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;
	}

	if (IsValid(ActiveWidget) && ActiveWidget != MainMenuWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	if (IsValid(MainMenuWidget))
	{
		ActiveWidget = MainMenuWidget;
		ActiveWidget->AddMenuToScreen();

		// @todo load main menu widget state from player save game
	}
}

void AMainMenuPlayerController::SwitchToNewProfileCreationWidget_Implementation()
{
	if (IsValid(ActiveWidget) && ActiveWidget != NewProfileCreationWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	CreateNewProfileCreationWidget();

	if (IsValid(NewProfileCreationWidget))
	{
		ActiveWidget = NewProfileCreationWidget;
		ActiveWidget->AddMenuToScreen();
	}
}

void AMainMenuPlayerController::SwitchToMultiplayerWidget_Implementation()
{
	if (IsValid(ActiveWidget) && ActiveWidget != MultiplayerWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	CreateMultiplayerWidget();

	if (IsValid(MultiplayerWidget))
	{
		ActiveWidget = MultiplayerWidget;
		ActiveWidget->AddMenuToScreen();
	}
}

void AMainMenuPlayerController::SwitchToSettingsWidget_Implementation()
{
	if (IsValid(ActiveWidget) && ActiveWidget != SettingsWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	CreateSettingsWidget();

	if (IsValid(SettingsWidget))
	{
		ActiveWidget = SettingsWidget;
		ActiveWidget->AddMenuToScreen();
	}
}

void AMainMenuPlayerController::StartNewCampaign()
{
	UGameplayStatics::OpenLevel(this, FName("Level0_Haddon"));
}

void AMainMenuPlayerController::CreateAndLoadNewProfile(const FString& NewProfileName)
{
	UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
	if (IsValid(GameInstance) && IsValid(ActiveWidget) && ActiveWidget == NewProfileCreationWidget)
	{
		GameInstance->CreateNewProfile(NewProfileName);
		UPlayerSaveGame* PlayerSaveGame = GameInstance->LoadProfileAsCurrent(NewProfileName);
		if (IsValid(PlayerSaveGame))
		{
			SwitchToMainMenuWidget(PlayerSaveGame);
		}
	}
}

void AMainMenuPlayerController::HandleTitleScreenAnyKeyEvent_Implementation(const FKey& Key)
{
	if (Key.IsMouseButton() || Key == EKeys::Escape || ActiveWidget == nullptr || (IsValid(ActiveWidget) && ActiveWidget != TitleScreenWidget))
	{
		return;
	}

	UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
	UPlayerSaveGame* PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;

	if (IsValid(PlayerSaveGame))
	{
		SwitchToMainMenuWidget(PlayerSaveGame);
	}
	else
	{
		SwitchToNewProfileCreationWidget();
	}
}
