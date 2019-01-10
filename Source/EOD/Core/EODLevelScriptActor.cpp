// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLevelScriptActor.h"
#include "EOD/Characters/EODCharacterBase.h"
#include "EOD/Characters/PlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AEODLevelScriptActor::AEODLevelScriptActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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
