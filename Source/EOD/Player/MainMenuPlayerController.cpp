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

void AMainMenuPlayerController::CreateAndLoadNewProfile(const FString& NewProfileName)
{
	UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
	if (IsValid(GameInstance) && IsValid(ActiveWidget) && ActiveWidget == CreateNewProfileWidget)
	{
		ActiveWidget->RemoveFromParent();

		GameInstance->CreateNewProfile(NewProfileName);
		UPlayerSaveGame* PlayerSaveGame = GameInstance->LoadProfileAsCurrent(NewProfileName);
		if (IsValid(PlayerSaveGame))
		{
			if (!IsValid(MainMenuWidget) && MainMenuWidgetClass.Get())
			{
				MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
			}

			if (IsValid(MainMenuWidget))
			{
				ActiveWidget = MainMenuWidget;
				ActiveWidget->AddToViewport();
			}
		}
	}

}

void AMainMenuPlayerController::HandleTitleScreenAnyKeyEvent(const FKey& Key)
{
	if (Key.IsMouseButton() || Key == EKeys::Escape)
	{
		return;
	}

	if (IsValid(ActiveWidget) && ActiveWidget == TitleScreenWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}


	UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
	UPlayerSaveGame* PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;
	if (IsValid(PlayerSaveGame))
	{
		if (!IsValid(MainMenuWidget) && MainMenuWidgetClass.Get())
		{
			MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		}

		if (IsValid(MainMenuWidget))
		{
			ActiveWidget = MainMenuWidget;
			ActiveWidget->AddToViewport();
		}
	}
	else
	{
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
}
