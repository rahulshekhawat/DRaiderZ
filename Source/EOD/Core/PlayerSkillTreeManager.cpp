// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerSkillTreeManager.h"
#include "SkillTreeWidget.h"
#include "EODGameInstance.h"

APlayerSkillTreeManager::APlayerSkillTreeManager(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);
	SetReplicateMovement(false);

}

void APlayerSkillTreeManager::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerSkillTreeManager::Destroyed()
{
	Super::Destroyed();
}

void APlayerSkillTreeManager::CreateSkillTreeWidget(UEODGameInstance* GameInstance)
{
	UGameInstance* GInst = GameInstance ? GameInstance : GetGameInstance();
	if (GInst)
	{
		SkillTreeWidget = CreateWidget<USkillTreeWidget>(GInst, SkillTreeWidgetClass);
	}
}
