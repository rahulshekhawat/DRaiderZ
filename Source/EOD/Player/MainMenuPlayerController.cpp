// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Player/MainMenuPlayerController.h"
#include "EOD/Core/EODGameInstance.h"
#include "EOD/SaveSystem/PlayerSaveGame.h"

AMainMenuPlayerController::AMainMenuPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// PrimaryActorTick.bCanEverTick = false;
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController())
	{
		SwitchToUIInput();
		if (TitleScreenWidgetClass.Get())
		{
			TitleScreenWidget = CreateWidget<UUserWidget>(this, TitleScreenWidgetClass);
			if (IsValid(TitleScreenWidget))
			{
				ActiveWidget = TitleScreenWidget;
				ActiveWidget->AddToViewport();
			}
		}
	}
}

void AMainMenuPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainMenuPlayerController::SwitchToTitleScreenWidget()
{
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
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	if (!IsValid(MainMenuWidget) && MainMenuWidgetClass.Get())
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	}

	if (IsValid(MainMenuWidget))
	{
		ActiveWidget = MainMenuWidget;
		ActiveWidget->AddToViewport();

		// @todo load main menu widget state from player save game
	}
}

void AMainMenuPlayerController::SwitchToCreateNewProfileWidget()
{
	if (IsValid(ActiveWidget) && ActiveWidget != CreateNewProfileWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	if (!IsValid(CreateNewProfileWidget) && CreateNewProfileWidgetClass.Get())
	{
		CreateNewProfileWidget = CreateWidget<UUserWidget>(this, CreateNewProfileWidgetClass);
	}

	if (IsValid(CreateNewProfileWidget))
	{
		ActiveWidget = CreateNewProfileWidget;
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

	if (!IsValid(MultiplayerWidget) && MultiplayerWidgetClass.Get())
	{
		MultiplayerWidget = CreateWidget<UUserWidget>(this, MultiplayerWidgetClass);
	}

	if (IsValid(MultiplayerWidget))
	{
		ActiveWidget = MultiplayerWidget;
		ActiveWidget->AddToViewport();
	}
}

void AMainMenuPlayerController::CreateAndLoadNewProfile(const FString& NewProfileName)
{
	UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
	if (IsValid(GameInstance) && IsValid(ActiveWidget) && ActiveWidget == CreateNewProfileWidget)
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
	if (Key.IsMouseButton() || Key == EKeys::Escape)
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
		SwitchToCreateNewProfileWidget();
	}
}
