// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"
#include "GameplaySkillBase.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "DynamicHUDWidget.h"
#include "DynamicSkillTreeWidget.h"

USkillTreeComponent::USkillTreeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USkillTreeComponent::InitializeSkillTreeWidget()
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	UDynamicHUDWidget* HUDWidget = PC ? PC->GetHUDWidget() : nullptr;
	SkillTreeWidget = HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(PC->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	if (SkillTreeWidget && SaveGame)
	{
		SkillPointsInfoWidget = SkillTreeWidget->GetSkillPointsInfoWidget();
		if (SaveGame)
		{
			SkillTreeWidget->InitializeSkillTreeLayout(SkillTreeLayoutTable, SaveGame->SkillTreeSlotsSaveData);
		}
		else
		{
			SkillTreeWidget->InitializeSkillTreeLayout(SkillTreeLayoutTable);
		}
	}
}
