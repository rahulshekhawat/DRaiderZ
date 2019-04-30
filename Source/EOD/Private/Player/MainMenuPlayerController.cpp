// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MainMenuPlayerController.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "EODLevelScriptActor.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AMainMenuPlayerController::AMainMenuPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	LevelSequenceTag = FName("MainMenuLevelSequence");
	CameraActorTag = FName("MainMenuCamera");

	/*
	MainMenuAudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("MainMenuMusicComponent"));
	if (MainMenuAudioComponent)
	{
		MainMenuAudioComponent->SetupAttachment(RootComponent);
	}
	*/
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

	LevelScriptActor = GetEODLevelScriptActor();
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
	}

	if (LevelScriptActor)
	{
		LevelScriptActor->FadeInViewport();
		LevelScriptActor->SwitchToBGM(MainMenuMusic);
	}

	if (MainMenuCameraActor == nullptr)
	{
		MainMenuCameraActor = GetMainMenuCameraActor();
	}
	if (MainMenuCameraActor)
	{
		SetViewTargetWithBlend(MainMenuCameraActor);
	}

	//~ @todo Play LoginIdle animation

	/*
	if (MainMenuAudioComponent && MainMenuMusic)
	{
		MainMenuAudioComponent->SetSound(MainMenuMusic);
		MainMenuAudioComponent->Play();
	}
	*/
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

AEODLevelScriptActor* AMainMenuPlayerController::GetEODLevelScriptActor() const
{
	UWorld* World = GetWorld();
	UClass* ActorClass = AEODLevelScriptActor::StaticClass();

	// We do nothing if no is class provided, rather than giving ALL actors!
	if (World)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			AEODLevelScriptActor* LevelActor = Cast<AEODLevelScriptActor>(*It);
			if (IsValid(LevelActor))
			{
				return LevelActor;
			}
		}
	}

	return nullptr;
}

ACameraActor* AMainMenuPlayerController::GetMainMenuCameraActor() const
{
	UWorld* World = GetWorld();
	UClass* ActorClass = ACameraActor::StaticClass();

	// We do nothing if no is class provided, rather than giving ALL actors!
	if (World)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			ACameraActor* CameraActor = Cast<ACameraActor>(*It);
			if (IsValid(CameraActor) && CameraActor->Tags.Contains(CameraActorTag))
			{
				return CameraActor;
			}
		}
	}

	return nullptr;
}

ALevelSequenceActor* AMainMenuPlayerController::GetMainMenuLevelSeqActor() const
{
	UWorld* World = GetWorld();
	UClass* ActorClass = ALevelSequenceActor::StaticClass();

	// We do nothing if no is class provided, rather than giving ALL actors!
	if (World)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			ALevelSequenceActor* SeqActor = Cast<ALevelSequenceActor>(*It);
			if (IsValid(SeqActor) && SeqActor->Tags.Contains(CameraActorTag))
			{
				return SeqActor;
			}
		}
	}

	return nullptr;
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
