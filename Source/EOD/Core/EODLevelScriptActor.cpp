// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLevelScriptActor.h"
#include "EOD/Player/PlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AEODLevelScriptActor::AEODLevelScriptActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AEODLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->GetPawn())
	{
		APlayerCharacter* PlayerPawn = Cast<APlayerCharacter>(PC->GetPawn());
		if (PlayerPawn)
		{
			PlayerPawn->CombatInitiated.AddDynamic(this, &AEODLevelScriptActor::CombatStarted);
			PlayerPawn->CombatFinished.AddDynamic(this, &AEODLevelScriptActor::CombatEnded);
		}
	}
}

void AEODLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODLevelScriptActor::CombatStarted_Implementation()
{
}

void AEODLevelScriptActor::CombatEnded_Implementation()
{
}
