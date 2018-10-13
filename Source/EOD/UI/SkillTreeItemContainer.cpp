// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeItemContainer.h"
#include "Core/EODSaveGame.h"
#include "Core/GameSingleton.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

USkillTreeItemContainer::USkillTreeItemContainer(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EEODContainerType::SkillTree;
}

bool USkillTreeItemContainer::Initialize()
{
	if (!(Super::Initialize() &&
		SkillUpgradeText))
	{
		return false;
	}

	LoadSkillState();

	return true;
}

void USkillTreeItemContainer::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillTreeItemContainer::NativeDestruct()
{
	Super::NativeDestruct();
}

FORCEINLINE void USkillTreeItemContainer::LoadSkillState()
{
	if (SkillGroup == FString())
	{
		return;
	}

	UGameSingleton* GameSingleton = nullptr;
	if (GEngine)
	{
		GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	}

	if (!GameSingleton)
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex));
	if (!EODSaveGame)
	{
		return;
	}

	if (EODSaveGame->SkillToStateMap.Contains(SkillGroup))
	{
		SkillState = EODSaveGame->SkillToStateMap[SkillGroup];
	}
}
