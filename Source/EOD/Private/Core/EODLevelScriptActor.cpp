// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLevelScriptActor.h"
#include "EODCharacterBase.h"
#include "PlayerCharacter.h"

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
}

void AEODLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	APlayerCharacter* PlayerPawn = PC ? Cast<APlayerCharacter>(PC->GetPawn()) : nullptr;
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->OnInitiatingCombat.AddDynamic(this, &AEODLevelScriptActor::OnCombatStarted);
		PlayerPawn->OnLeavingCombat.AddDynamic(this, &AEODLevelScriptActor::OnCombatEnded);
	}
}

void AEODLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODLevelScriptActor::OnCombatStarted_Implementation(AEODCharacterBase* PlayerCharacter)
{
}

void AEODLevelScriptActor::OnCombatEnded_Implementation(AEODCharacterBase* PlayerCharacter)
{
}
