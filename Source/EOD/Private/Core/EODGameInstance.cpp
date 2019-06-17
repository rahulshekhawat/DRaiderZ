// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODGameInstance.h"
#include "EODPlayerController.h"
#include "MetaSaveGame.h"
#include "PlayerSaveGame.h"
#include "EODGlobalNames.h"
#include "DamageNumberWidget.h"

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Modules/ModuleManager.h"
#include "Kismet/GameplayStatics.h"


const int32 UEODGameInstance::PlayerIndex(0);
const FString UEODGameInstance::MetaSaveSlotName(TEXT("MetaSaveSlot"));

UEODGameInstance::UEODGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameTitle = FText::FromString("Dark RaiderZ");
	StartupMapName = FName("Level0_Haddon");

	CamShakeInnerRadius = 500.f;
	CamShakeOuterRadius = 1000.f;
}

void UEODGameInstance::Init()
{
	Super::Init();

	LoadSaveGame();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UEODGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UEODGameInstance::OnPostLoadMap);

}

void UEODGameInstance::StartNewCampaign()
{
	UGameplayStatics::OpenLevel(this, StartupMapName);
}

void UEODGameInstance::ContinuePreviousCampaign()
{
}

bool UEODGameInstance::CanContinuePreviousCampaign() const
{
	return false;
}

void UEODGameInstance::CreateNewProfile(const FString& ProfileName)
{
	if (IsValid(MetaSaveGame))
	{
		UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));
		if (IsValid(PlayerSaveGame))
		{
			bool bResult = UGameplayStatics::SaveGameToSlot(PlayerSaveGame, ProfileName, UEODGameInstance::PlayerIndex);
			if (bResult)
			{
				FMetaSaveGameData TempMSGData;
				TempMSGData.SaveSlotName = ProfileName;
				TempMSGData.CharacterName = ProfileName;
				TempMSGData.PlayerIndex = UEODGameInstance::PlayerIndex;
				TempMSGData.LastSaveTime = FDateTime::Now();

				MetaSaveGame->SaveSlotMetaDataList.Add(TempMSGData);
				UGameplayStatics::SaveGameToSlot(MetaSaveGame, UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex);
			}
		}
	}

}

UPlayerSaveGame* UEODGameInstance::LoadProfileAsCurrent(const FString& ProfileName)
{
	if (ProfileName == CurrentProfileName)
	{
		return CurrentProfileSaveGame;
	}
	else
	{
		if (IsValid(MetaSaveGame))
		{
			UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(ProfileName, UEODGameInstance::PlayerIndex));
			if (IsValid(PlayerSaveGame))
			{
				CurrentProfileSaveGame = PlayerSaveGame;
				CurrentProfileName = ProfileName;
				MetaSaveGame->LastUsedSlotName = ProfileName;
				UGameplayStatics::SaveGameToSlot(MetaSaveGame, UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex);
				return CurrentProfileSaveGame;
			}
		}
	}

	return nullptr;
}

void UEODGameInstance::LoadSaveGame()
{
	// Load the meta save game object
	if (UGameplayStatics::DoesSaveGameExist(UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex))
	{
		MetaSaveGame = Cast<UMetaSaveGame>(UGameplayStatics::LoadGameFromSlot(UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex));
	}
	// If the meta save game object doesn't exist yet (running game for first time), create and save one 
	else
	{
		MetaSaveGame = Cast<UMetaSaveGame>(UGameplayStatics::CreateSaveGameObject(UMetaSaveGame::StaticClass()));
		if (IsValid(MetaSaveGame))
		{
			UGameplayStatics::SaveGameToSlot(MetaSaveGame, UEODGameInstance::MetaSaveSlotName, UEODGameInstance::PlayerIndex);
		}
	}

	if (IsValid(MetaSaveGame))
	{
		if (MetaSaveGame->LastUsedSlotName != FString("") && UGameplayStatics::DoesSaveGameExist(MetaSaveGame->LastUsedSlotName, UEODGameInstance::PlayerIndex))
		{
			CurrentProfileSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(MetaSaveGame->LastUsedSlotName, UEODGameInstance::PlayerIndex));
			CurrentProfileName = MetaSaveGame->LastUsedSlotName;
		}
#if WITH_EDITOR
		else
		{
			CreateNewProfile(FString("Chikara"));
			LoadProfileAsCurrent(FString("Chikara"));
		}
#endif
	}
}

void UEODGameInstance::OnPreLoadMap(const FString& MapName)
{
	IGameMoviePlayer* MoviePlayer = GetMoviePlayer();

	if (!MoviePlayer || MoviePlayer->IsStartupMoviePlaying() || IsRunningDedicatedServer())
	{
		return;
	}
	
	if (!IsValid(LoadingScreenWidget))
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidgetClass);
	}

	if (IsValid(LoadingScreenWidget))
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();


		MoviePlayer->SetupLoadingScreen(LoadingScreen);
	}
}

void UEODGameInstance::OnPostLoadMap(UWorld* WorldObj)
{
}

void UEODGameInstance::DisplayDamageNumbers(
	const float DamageValue,
	const bool bCritHit,
	const AActor* DamagedActor,
	const AActor* DamageInstigator,
	const FVector& HitLocation)
{
	UClass* WidgetClass = DamageWidgetClass.Get();
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetFirstLocalPlayerController());
	if (!PC || !WidgetClass)
	{
		return;
	}

	APawn* ControlledPawn = PC->GetPawn();
	check(ControlledPawn);

	// If neither the damaged actor or damage instigator is a player controlled character, then we do not want to display damage
	if (ControlledPawn != DamagedActor && ControlledPawn != DamageInstigator)
	{
		return;
	}

	UDamageNumberWidget* DamageWidget = CreateWidget<UDamageNumberWidget>(PC, WidgetClass);
	if (DamageWidget)
	{
		FVector2D WidgetLocation;
		bool bResult = PC->ProjectWorldLocationToScreen(HitLocation, WidgetLocation, true);
		if (bResult)
		{
			DamageWidget->SetDamageValue(DamageValue);
			FLinearColor FinalColor;
			if (DamagedActor == ControlledPawn)
			{
				FinalColor = PlayerDamagedTextColor;
			}
			else
			{
				FinalColor = bCritHit ? NPCCritDamagedTextColor : NPCNormalDamagedTextColor;
			}
			DamageWidget->WorldPosition = HitLocation;
			DamageWidget->SetDamageColor(FinalColor);
			DamageWidget->SetPositionInViewport(WidgetLocation);
			DamageWidget->AddToViewport(-10);
		}
	}
}

void UEODGameInstance::PlayCameraShake(ECameraShakeType CameraShakeType, const FVector& EpiCenter)
{
	switch (CameraShakeType)
	{
	case ECameraShakeType::Weak:
		UGameplayStatics::PlayWorldCameraShake(this, LowCamShakeClass, EpiCenter, CamShakeInnerRadius, CamShakeOuterRadius);
		break;
	case ECameraShakeType::Medium:
		UGameplayStatics::PlayWorldCameraShake(this, MediumCamShakeClass, EpiCenter, CamShakeInnerRadius, CamShakeOuterRadius);
		break;
	case ECameraShakeType::Strong:
		UGameplayStatics::PlayWorldCameraShake(this, HighCamShakeClass, EpiCenter, CamShakeInnerRadius, CamShakeOuterRadius);
		break;
	default:
		break;
	}
}

void UEODGameInstance::DisplayGameplayText(
	const FString& GameplayText,
	const AActor* DamagedActor,
	const AActor* DamageInstigator,
	const FVector& HitLocation)
{
	//~ @todo
}
