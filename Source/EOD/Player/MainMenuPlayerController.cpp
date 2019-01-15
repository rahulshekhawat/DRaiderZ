// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "MainMenuPlayerController.h"

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

void AMainMenuPlayerController::HandleAnyKeyEvent(const FKey& Key)
{
	if (Key.IsMouseButton() || Key == EKeys::Escape)
	{
		return;
	}

	if (ActiveWidget == TitleScreenWidget)
	{
		if (CreateNewProfileWidgetClass.Get())
		{
			ActiveWidget->RemoveFromParent();
			CreateNewProfileWidget = CreateWidget<UUserWidget>(this, CreateNewProfileWidgetClass);
			if (IsValid(CreateNewProfileWidget))
			{
				ActiveWidget = CreateNewProfileWidget;
				ActiveWidget->AddToViewport();
			}
		}


		/*
		if (MainMenuWidgetClass.Get())
		{
			TitleScreenWidget->RemoveFromParent();
			MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
			if (IsValid(MainMenuWidget))
			{
				ActiveWidget = MainMenuWidget;
				ActiveWidget->AddToViewport();
			}
		}
		*/
	}


}
