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

void AMainMenuPlayerController::SwitchToTitleScreenWidget()
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

void AMainMenuPlayerController::SwitchToMainMenuWidget(UPlayerSaveGame* PlayerSaveGame)
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

	// CreateMainMenuWidget();

	if (IsValid(MainMenuWidget))
	{
		ActiveWidget = MainMenuWidget;
		ActiveWidget->AddMenuToScreen();

		// @todo load main menu widget state from player save game
	}
}

void AMainMenuPlayerController::SwitchToNewProfileCreationWidget()
{
	if (IsValid(ActiveWidget) && ActiveWidget != NewProfileCreationWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	CreateNewProfileCreationWidget();

	if (IsValid(NewProfileCreationWidget))
	{
		ActiveWidget = NewProfileCreationWidget;
		ActiveWidget->AddToViewport();
	}
}

void AMainMenuPlayerController::SwitchToMultiplayerWidget()
{
	if (IsValid(ActiveWidget) && ActiveWidget != MultiplayerWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	CreateMultiplayerWidget();

	if (IsValid(MultiplayerWidget))
	{
		ActiveWidget = MultiplayerWidget;
		ActiveWidget->AddToViewport();
	}
}

void AMainMenuPlayerController::SwitchToSettingsWidget()
{
	if (IsValid(ActiveWidget) && ActiveWidget != SettingsWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	CreateSettingsWidget();

	if (IsValid(SettingsWidget))
	{
		ActiveWidget = SettingsWidget;
		ActiveWidget->AddToViewport();
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

void AMainMenuPlayerController::HandleTitleScreenAnyKeyEvent(const FKey& Key)
{
	if (Key.IsMouseButton() || Key == EKeys::Escape || (IsValid(ActiveWidget) && ActiveWidget != TitleScreenWidget))
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

void AMainMenuPlayerController::CreatePlayerMenu()
{
	if (IsLocalPlayerController())
	{

		if (IsValid(TitleScreenWidget))
		{
			ActiveWidget = TitleScreenWidget;
			ActiveWidget->AddToViewport();
			
			// TitleScreenWidget->OnAd
		}

		SwitchToUIInput();
	}
}
