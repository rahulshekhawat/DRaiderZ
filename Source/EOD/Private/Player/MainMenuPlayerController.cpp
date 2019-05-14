// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MainMenuPlayerController.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "MainMenuLevelScriptActor.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
	
	LevelScriptActor = GetMainMenuLevelScriptActor();
	check(LevelScriptActor);
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
	check(TitleScreenWidget);

	// If title screen widget is already active
	if (ActiveWidget && ActiveWidget == TitleScreenWidget)
	{
		return;
	}

	if (ActiveWidget && ActiveWidget != TitleScreenWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	ActiveWidget = TitleScreenWidget;
	ActiveWidget->AddMenuToScreen();
}

void AMainMenuPlayerController::SwitchToMainMenuWidget_Implementation(UPlayerSaveGame* PlayerSaveGame)
{
	check(MainMenuWidget);

	// If main menu widget is already active
	if (ActiveWidget && ActiveWidget == MainMenuWidget)
	{
		return;
	}

	// If a proper player save game was not passed as an argument, we try to get the current player save game object from game instance
	if (PlayerSaveGame)
	{
		UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
		PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;
	}

	if (ActiveWidget && ActiveWidget != MainMenuWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	ActiveWidget = MainMenuWidget;
	ActiveWidget->AddMenuToScreen();

	if (LevelScriptActor)
	{
		LevelScriptActor->OnSwitchingToMainMenu();
	}
}

void AMainMenuPlayerController::SwitchToNewProfileCreationWidget_Implementation()
{
	check(NewProfileCreationWidget);

	// If title screen widget is already active
	if (ActiveWidget && ActiveWidget == NewProfileCreationWidget)
	{
		return;
	}

	if (ActiveWidget && ActiveWidget != NewProfileCreationWidget)
	{
		ActiveWidget->RemoveMenuFromScreen();
		ActiveWidget = nullptr;
	}

	ActiveWidget = NewProfileCreationWidget;
	ActiveWidget->AddMenuToScreen();
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

AMainMenuLevelScriptActor* AMainMenuPlayerController::GetMainMenuLevelScriptActor() const
{
	UWorld* World = GetWorld();
	UClass* ActorClass = AMainMenuLevelScriptActor::StaticClass();

	// We do nothing if no is class provided, rather than giving ALL actors!
	if (World)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			AMainMenuLevelScriptActor* LevelActor = Cast<AMainMenuLevelScriptActor>(*It);
			if (IsValid(LevelActor))
			{
				return LevelActor;
			}
		}
	}

	return nullptr;
}

void AMainMenuPlayerController::StartNewCampaign()
{
	// UGameplayStatics::OpenLevel(this, FName("Level0_Haddon"));
	UGameplayStatics::OpenLevel(this, FName("RietZ"));
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
