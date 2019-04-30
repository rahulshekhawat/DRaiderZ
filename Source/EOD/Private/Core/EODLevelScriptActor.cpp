// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLevelScriptActor.h"
#include "EODCharacterBase.h"
#include "PlayerCharacter.h"
#include "MusicTriggerBox.h"

#include "UserWidget.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

const FName AEODLevelScriptActor::BGMAudioComponentName(TEXT("BGM Audio Component"));

AEODLevelScriptActor::AEODLevelScriptActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BGMAudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, AEODLevelScriptActor::BGMAudioComponentName);
	if (BGMAudioComponent)
	{
		BGMAudioComponent->SetupAttachment(RootComponent);
	}

	InitialBGMTriggerDelay = 5.f;
	QueuedBGMTriggerDelay = 10.f;
	BGMFadeInDuration = BGMFadeOutDuration = 3.f;
}

void AEODLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	BindCombatDelegates();
	SetupBindingsForMusicTriggers();

	DisplayLevelTitle();

	InitiateBackgroundMusic();

	FadeInViewport();
}

void AEODLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODLevelScriptActor::OnCombatStarted_Implementation(AEODCharacterBase* PlayerCharacter)
{
	USoundBase* MusicToPlay = GetRandomSound(CombatMusicCollection);
	SwitchToBGM(MusicToPlay);
}

void AEODLevelScriptActor::OnCombatEnded_Implementation(AEODCharacterBase* PlayerCharacter)
{
	USoundBase* MusicToPlay = nullptr;
	if (ActiveTriggerMusic)
	{
		MusicToPlay = ActiveTriggerMusic;
	}
	else
	{
		MusicToPlay = GetRandomSound(NonCombatMusicCollection);
	}
	SwitchToBGM(MusicToPlay);
}

void AEODLevelScriptActor::SwitchToBGM(USoundBase* NewBGM)
{
	check(BGMAudioComponent);

	// If the BGM is already playing
	if (BGMAudioComponent->Sound == NewBGM && BGMAudioComponent->IsPlaying())
	{
		return;
	}
	else if (BGMAudioComponent->Sound != NewBGM && BGMAudioComponent->IsPlaying())
	{
		StopCurrentMusic();
		UWorld* World = GetWorld();
		if (World)
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &AEODLevelScriptActor::PlayMusic, NewBGM);
			World->GetTimerManager().SetTimer(MusicTimerHandle, TimerDelegate, BGMFadeOutDuration, false);
		}
	}
	else if (!BGMAudioComponent->IsPlaying())
	{
		PlayMusic(NewBGM);
	}
}

void AEODLevelScriptActor::PlayMusic(USoundBase* Music)
{
	UWorld* World = GetWorld();
	if (Music && BGMAudioComponent && World)
	{
		BGMAudioComponent->SetSound(Music);
		BGMAudioComponent->FadeIn(BGMFadeInDuration);

		FTimerHandle TempHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AEODLevelScriptActor::StopMusic, Music);
		float Duration = Music->GetDuration() - BGMFadeOutDuration;
		World->GetTimerManager().SetTimer(TempHandle, TimerDelegate, Duration, false);
	}
}

void AEODLevelScriptActor::StopMusic(USoundBase* Music)
{
	if (BGMAudioComponent && BGMAudioComponent->Sound == Music && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->FadeOut(BGMFadeOutDuration, 0.2f);
	}
}

void AEODLevelScriptActor::StopCurrentMusic()
{
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->FadeOut(BGMFadeOutDuration, 0.f);
	}
}

void AEODLevelScriptActor::InitiateBackgroundMusic()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle TempTimerHandle;
		World->GetTimerManager().SetTimer(TempTimerHandle, this, &AEODLevelScriptActor::PlayNonCombatMusic, InitialBGMTriggerDelay, false);
	}
}

void AEODLevelScriptActor::PlayCombatMusic()
{
	UWorld* World = GetWorld();
	USoundBase* MusicToPlay = GetRandomSound(CombatMusicCollection);
	if (MusicToPlay && World && BGMAudioComponent)
	{
		BGMAudioComponent->SetSound(MusicToPlay);
		BGMAudioComponent->FadeIn(BGMFadeInDuration);
		float Duration = MusicToPlay->GetDuration() - BGMFadeOutDuration;
		//~ @todo
		// World->GetTimerManager().SetTimer(MusicTimerHandle, this, &AEODLevelScriptActor::FadeOutActiveBGM, Duration, false);
	}
}

void AEODLevelScriptActor::PlayNonCombatMusic()
{
	USoundBase* MusicToPlay = GetRandomSound(NonCombatMusicCollection);
	PlayMusic(MusicToPlay);

	/*
	UWorld* World = GetWorld();
	USoundBase* MusicToPlay = GetRandomSound(NonCombatMusicCollection);
	if (MusicToPlay && World && BGMAudioComponent)
	{


		BGMAudioComponent->SetSound(MusicToPlay);
		BGMAudioComponent->FadeIn(BGMFadeInDuration);
		float Duration = MusicToPlay->GetDuration() - BGMFadeOutDuration;
		World->GetTimerManager().SetTimer(MusicTimerHandle, this, &AEODLevelScriptActor::QueueNextNonCombatMusic, Duration, false);
	}
	*/
}

void AEODLevelScriptActor::QueueNextNonCombatMusic()
{
	if (BGMAudioComponent)
	{
		BGMAudioComponent->FadeOut(BGMFadeOutDuration, 0.f);
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(MusicTimerHandle, this, &AEODLevelScriptActor::PlayNonCombatMusic, QueuedBGMTriggerDelay, false);
		}
	}
}

void AEODLevelScriptActor::BindCombatDelegates()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	APlayerCharacter* PlayerPawn = PC ? Cast<APlayerCharacter>(PC->GetPawn()) : nullptr;
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->OnInitiatingCombat.AddDynamic(this, &AEODLevelScriptActor::OnCombatStarted);
		PlayerPawn->OnLeavingCombat.AddDynamic(this, &AEODLevelScriptActor::OnCombatEnded);
	}
}

void AEODLevelScriptActor::DisplayLevelTitle()
{
	UClass* WidgetClass = LevelTitleWidgetClass.Get();
	if (WidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		UUserWidget* LevelTitleWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
		if (LevelTitleWidget)
		{
			LevelTitleWidget->AddToViewport(1000);
		}
	}
}

void AEODLevelScriptActor::SetupBindingsForMusicTriggers()
{
	UWorld* World = GetWorld();
	UClass* ActorClass = AMusicTriggerBox::StaticClass();
	if (World)
	{
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			AMusicTriggerBox* MusicTriggerBox = Cast<AMusicTriggerBox>(*It);
			if (IsValid(MusicTriggerBox))
			{
				MusicTriggerBox->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEODLevelScriptActor::OnMusicTriggerBeginOverlap);
				MusicTriggerBox->GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AEODLevelScriptActor::OnMusicTriggerEndOverlap);
			}
		}
	}
}

void AEODLevelScriptActor::OnMusicTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerController* FirstPC = UGameplayStatics::GetPlayerController(this, 0);
	APawn* PlayerPawn = FirstPC ? FirstPC->GetPawn() : nullptr;
	if (PlayerPawn != OtherActor)
	{
		// No need to process the overlap event if the OtherActor is not a local player character
		return;
	}

	AMusicTriggerBox* TriggerBox = OverlappedComponent ? Cast<AMusicTriggerBox>(OverlappedComponent->GetOwner()) : nullptr;
	USoundBase* MusicToPlay = TriggerBox ? TriggerBox->GetRandomMusic() : nullptr;
	if (MusicToPlay)
	{
		SwitchToBGM(MusicToPlay);
		ActiveMusicTriggerBox = TriggerBox;
		ActiveTriggerMusic = MusicToPlay;
	}
}

void AEODLevelScriptActor::OnMusicTriggerEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AMusicTriggerBox* TriggerBox = OverlappedComponent ? Cast<AMusicTriggerBox>(OverlappedComponent->GetOwner()) : nullptr;
	if (TriggerBox == ActiveMusicTriggerBox)
	{
		ActiveMusicTriggerBox = nullptr;
		ActiveTriggerMusic = nullptr;
	}
}
